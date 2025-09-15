// GameUIPolicy.cpp

#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIPolicy)

void UGameUIPolicy::AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// CommonUserWidget에 PlayerContext를 설정
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));

	// 해당 Layout에 우선순위를 높게 설정. 일단 매직넘버 1000
	// AddToPlayerScreen() 함수를 통해 Widget을 붙인다.
	Layout->AddToPlayerScreen(1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// UCommonUserWidget의 SlateWidget을 가져와서 Parent와 연결고리를 끊어 놓음.
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
	}
}

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	// PlayerController가 업데이트되면 GameLayout을 업데이트해주기 위해 Delegate를 추가.
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, 
		[this](UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
		{
			// 우선 추가된 Player가 있으면 제거.
			NotifyPlayerRemoved(LocalPlayer);

			// RootViewportLayouts를 순회하며 검색.
			// FRootViewportLayoutInfo의 operator==를 정의해야함.
			if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
			{
				// Layout만 업데이트해줌.
				AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
				LayoutInfo->bAddedToViewport = true;
			}
			else
			{
				// Layout을 생성하고 활성화.
				CreateLayoutWidget(LocalPlayer);
			}
		}
	);

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else 
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		// FindByKey() 함수를 사용하기위해 operator==() 재정의함.
	
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);

		// 비활성화를 확인하는 용도로 끔.
		LayoutInfo->bAddedToViewport = false;
	}
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	// Player가 Destroy되므로, OnPlayerControllerSet에서 제거.
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);

	// RootViewportLayouts에서 제거.
	const int32 LayoutInfoIndex = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (INDEX_NONE != LayoutInfoIndex)
	{
		// 만약 PrimaryGameLayout이 있으면 Viewport에서도 제거.
		UPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIndex].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIndex);
		RemoveLayoutFromViewport(LocalPlayer, Layout);
	}
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
	// PlayerController가 할당되었을 경우, LayoutWidget을 생성한다.
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		// LayoutWidgetClass가 있을 경우. UPrimaryGameLayout은 Abstract 클래스이므로 이를 상속 받는 클래스여야 함.
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && false == LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			// PlayerController가 소유한다는 의미에서 Owner를 설정
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			// FRootViewportLayoutInfo의 Constructor를 정의.
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

UPrimaryGameLayout* UGameUIPolicy::GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}
