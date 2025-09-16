// GFA_AddWidget.cpp

#include "GFA_AddWidget.h"
#include "UI/LyraHUD.h"
#include "CommonGame/CommonUIExtension.h"
#include "UIExtension/UIExtensionSubsystem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GFA_AddWidget)

void UGFA_AddWidget::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraHUD* HUD = CastChecked<ALyraHUD>(Actor);

	// HUD를 통해 LocalPlayer를 가져오자.
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		// Layout의 요청을 순회하자. 보통 하나만 들어있긴함.
		for (const FLyraHUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				ActiveData.LayoutsAdded.Add(UCommonUIExtension::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		// Widget을 순회하며 UIExtensionSubsystem의 Extension에 추가함.
		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FLyraHUDElementEntry& Entry : Widgets)
		{
			//ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}

void UGFA_AddWidget::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ALyraHUD* HUD = CastChecked<ALyraHUD>(Actor);

	// LyraHUD에 추가된 CommonActivatableWidget을 순회하며 Deactivate 시켜줌.
	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if (true == AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}
	ActiveData.LayoutsAdded.Reset();

	// UIExtension에 대해 순회하며 Unregister() 함수 호출.
	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		// Unregister() 함수는 UUIExtensionSystem에서 제거가 올바르게 진행되어야 함.
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();

}

void UGFA_AddWidget::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	ActiveData.LayoutsAdded.Empty();

	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGFA_AddWidget::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// Receiver인 ALyraHUD의 Removed/Added에 따라 Widget을 추가/제거 해줌.
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidgets(Actor, ActiveData);
	}
}

void UGFA_AddWidget::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// GameFrameworkComponentManager를 가져올 GameInstance와 World는 GameWorld여야 함.
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		// GameFrameworkComponentManager를 가져옴
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			// 기본적으로 Widget을 추가할 대상으로 ALyraHUD를 고정.
			TSoftClassPtr<AActor> HUDActorClass = ALyraHUD::StaticClass();

			// GameFeatureAction_WorldBase와 마찬가지로 HandleActorExtension을 콜백으로 받도록 하자.
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext)
			);

			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}

}

void UGFA_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}
