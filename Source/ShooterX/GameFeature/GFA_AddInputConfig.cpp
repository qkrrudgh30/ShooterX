// GFA_AddInputConfig.cpp


#include "GameFeature/GFA_AddInputConfig.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/LyraHeroComponent.h"

void UGFA_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) || !ensure(ActiveData.PawnsAddedTo.IsEmpty()))
	{
		Reset(ActiveData);
	}

	Super::OnGameFeatureActivating(Context);
		// UGameFeatureAction_WorldActionBase를 호출하면서 AddToWorld() 호출
}

void UGFA_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	
	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}	
}

void UGFA_AddInputConfig::AddToWorld(const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if (GameInstance && World && World->IsGameWorld())
	{
		// GFCM을 이용하여 ExtensionHandler를 추가하여 등록 진행
		// - HandlePawnExtension 콜백 함수로 연결
		if (UGameFrameworkComponentManager* ComponentMananger = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);

			// 등록된 콜백 함수의 핸들을 ActionData의 ExtensionRequestHandles에 등록.
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMananger->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);

			ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
		}
	}
}

void UGFA_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName,
	FGameFeatureStateChangeContext ChangeContext)
{
	APawn* AsPawn = CastChecked<APawn>(Actor);
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == ULyraHeroComponent::NAME_BindInputsNow)
	{
		AddInputConfig(AsPawn, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved)
	{
		RemoveInputConfig(AsPawn, ActiveData);
	}
}

void UGFA_AddInputConfig::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;

			int IMCPriority = 0;
			for (TSoftObjectPtr<UInputMappingContext> InputMappingContext : InputMappingContexts)
			{
				Subsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), IMCPriority++, Options);
			}

			// ActiveData에 Pawn을 관리대상으로 등록.
			ActiveData.PawnsAddedTo.AddUnique(Pawn);
		}
	}
}

void UGFA_AddInputConfig::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (TSoftObjectPtr<UInputMappingContext> InputMappingContext : InputMappingContexts)
			{
				Subsystem->RemoveMappingContext(InputMappingContext.LoadSynchronous());	
			}

			ActiveData.PawnsAddedTo.Remove(Pawn);
		}
	}
}

void UGFA_AddInputConfig::Reset(FPerContextData& ActiveData)
{
	// 게임피쳐서브시스템은 엔진과 생명주기를 같이함.
	// 엔진은 켜진 상태에서, PIE를 켜서 폰에 게임피쳐액션이 적용되었다고 해보자.
	// 다시 모종의 이유로 PIE가 꺼지고, 다시 PIE가 켜진다면 기존 폰에 대한 데이터가 남아있으면 안됨.
	// 기존 폰을 알고 있다면 GFA_AddInputConfig가 또 적용될 수 있음. 이걸 초기화해주는 함수.
	// 게임피쳐액션이 디액티베이트될 때 Reset() 함수로 호출해서 초기화 해줄 예정.

	// ExtensionRequestHandle를 초기화
	ActiveData.ExtensionRequestHandles.Empty();

	// PawnsAddedTo에 대해서 하나씩 Stack 방식으로 위에서 아래로 직접 InputConfig를 제거 진행
	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();
		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}
		else
		{
			//WeakObjectPtr로 PawnsAddedTo를 관리하고 있기 때문에 GC되었다면 nullptr일 수 있음.
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}
