// GameUIPolicy.h

#pragma once

#include "UObject/Object.h"
#include "GameUIPolicy.generated.h"

class UPrimaryGameLayout;
class UCommonLocalPlayer;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()

public:
	FRootViewportLayoutInfo()
	{

	}

	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{

	}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }

public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

};

UCLASS(Abstract, Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	void AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	
	void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer);

	void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);

	UPrimaryGameLayout* GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const;

public:
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;
	// LocalPlayer에 바인딩된 UI의 Layout으로 생각하면 됨. 아직 의미가 모호할 수 있는데, 하나씩 구현하면서 이해하도록하자.

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
	// 보통 싱글 게임에서는 LocalPlayer-PrimaryGameLayout 하나만 있겠지만 멀티플레이의 경우 복수개가 가능. 리플레이, ...
	// 이 속성을보면 알 수 있는 것이, UI를 관리하는 것은 결국 GameUIManager 개체가 아님. Policy임.
	// UI는 상당히 추상도가 높은 컨텐츠. 누가 구현하냐에 따라 너무나도 달라서 너만의 정책을 알아서 결정해라는 느낌.
	// 라이라 방식이 싫다면 GameUIPolicy를 상속 받아서 나만의 정책을 정하고 GameUIManagerSubsystem::DefaultUIPolicyClass에 지정해주면 됨.
	
};
