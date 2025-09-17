// GFA_AddWidget.h

#pragma once

#include "GFA_WorldActionBase.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Components/GameFrameworkComponentManager.h"
#include "UIExtension/UIExtensionSubsystem.h"
#include "GFA_AddWidget.generated.h"

// HUD의 Layout 요청
USTRUCT()
struct FLyraHUDLayoutRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;
	// UI의 레이아웃으로 CommonActivatableWidget을 사용.

	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTag LayerID;
	// 앞서 보았던 PrimaryGameLayout의 LayerID를 의미.

};

USTRUCT()
struct FLyraHUDElementEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=UI)
	TSoftClassPtr<UUserWidget> WidgetClass; 
	// LCHUDLayout 위에 올릴 대상이 되는 Widget Class
	// TSubclassOf<> 템플릿 클래스로 해야 무조건 로드이고, TSoftClassPtr<> 템플릿 클래스의 경우엔 필요할 때 로드함.
	// 이후에 meta=(AssetBundles="Client") 키워드를 추가함으로써 로드 로직이 수행되게끔 함.

	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTag SlotID;
	// SlotID는 LyraHUDLayoutRequest에 올린 LayoutClass에 정의된 Slot(GameplayTag)을 의미

};

UCLASS(meta=(DisplayName="Add Widget"))
class SHOOTERX_API UGFA_AddWidget : public UGFA_WorldActionBase
{
	GENERATED_BODY()

public:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

		// 라이라프로젝트에서 HUDElement는 UIExtension으로 관리된다.
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

public:
	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);

	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

	void Reset(FPerContextData& ActiveData);

	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

public:
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	// GameFeatureAction Add/Remove 상태 관리

	UPROPERTY(EditAnywhere, Category=UI)
	TArray<FLyraHUDLayoutRequest> Layout;
	// GFA_AddWidget은 형태를 정의하는 Layout과 Layout 위에 올릴 위젯 개체들(Widgets)로 구성됨.

	UPROPERTY(EditAnywhere, Category=UI)
	TArray<FLyraHUDElementEntry> Widgets;

};
