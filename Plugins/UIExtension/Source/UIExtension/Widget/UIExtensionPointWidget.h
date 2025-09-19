// UIExtensionPointWidget.h

#pragma once

#include "Components/DynamicEntryBoxBase.h"
#include "GameplayTagContainer.h"
#include "UIExtension/UIExtensionSystem.h"
#include "UIExtensionPointWidget.generated.h"

class UCommonLocalPlayer;

// UUIExtensionPointWidget은 UIExtension 하나 단위로 생각하면 됨.
// UIExtension에 결합된 Context 별로 관리하는 개체가 UIExtensionPoint(Handle)이다.
// 예로들어 해당 UIExtension Slot에 PlayerState/PlayerController/LocalPlayer와 같다. 다양한 Context로부터 Widget이 결합될 수 있다.
// 이를 분리하여 관리하기 위해 만들어진 개체가 UIExtensionPoint이다.
UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request);

	void ResetExtensionPoint();

	void RegisterExtensionPoint();

	void RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);

	TSharedRef<SWidget> RebuildWidget();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UIExtension")
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UIExtension")
	EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UIExtension")
	TArray<TObjectPtr<UClass>> DataClasses;
	// UIExtensionPointWidget에 결합이 허용된 UWidgetClasses
	// ExtensionPointTag에 맞는 위젯을 찾아왔는데, 원하는 위젯이랑 적용이 안될수도 있음. 그럼 크래시남.
	// 그래서 ExtensionPointWidget에서 정한 UClass(결국엔 위젯클래스)만 가능하도록 제한하는 역할의 속성.

	// UIExtension --- Widget 관계 매핑
	UPROPERTY(Transient)
	TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;
	// 해당 ExtensionPointWidget에는 위젯 하나만 넣을 수있는게 아니라 여러 개 가능함.

	// UIExtensionPoint 개체 관리
	TArray<FUIExtensionPointHandle> ExtensionPointHandles;

};
