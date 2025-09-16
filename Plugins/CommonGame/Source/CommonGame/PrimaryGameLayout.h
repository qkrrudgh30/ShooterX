// PrimaryGameLayout.h

#pragma once

#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "PrimaryGameLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

// 인게임에서 메인 UI의 레이아웃을 담당하는 UMG. (Slate는 UObject 기반이 아님. UMG는 UObject 기반.)
// PrimaryGameLayout은 플레이어당 하나씩 가지는 최상위 UI 레이아웃.
// PrimaryGameLayout은 레이어 기반으로 UI를 관리함.
UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);
	// 해당 클래스는 Abstract로 설정되었으므로 굳이 FObjectInitializer::Get() 함수를 호출할 필요가 없음.

	UFUNCTION(BlueprintCallable, Category="Layer")
	void RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);
	// 레이어를 추가하며 GameplayTag를 할당함.

	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName) { return Layers.FindRef(LayerName); }
	// LayerName에 따른 ActivatableWidgetContainerBase를 가져옴.

	template<typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&) {});
	}

	template<typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass, TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here.");

		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}

public:
	UPROPERTY(Transient, meta=(Categories="UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
	// GameplayTag --- CommonActivatableWidgetContainerBase
};
