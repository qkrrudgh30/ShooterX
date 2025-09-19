// CommonLocalPlayer.cpp

#include "CommonLocalPlayer.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonLocalPlayer)

UCommonLocalPlayer::UCommonLocalPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Super()의 인자로 FObjectInitializer::Get()를 넘겼음.
	// 이는 우리가 UCommonLocalPlayer() 생성자를 상속 받는 LocalPlayer 자식 클래스들은 FObjectInitalizer를 신경 쓰지 않게끔 하기 위함.
	// 만약 상속 받는 LocalPlayer 자식 클래스에서 FObjectInitializer를 통해 뭔가 추가 SubObject를 정의한다면 이와 같은 구현은 사용하면 안됨.
}

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;
	if (PlayerState)
	{
		// PlayerState가 설정되어 있으면 바로 호출
		Delegate.Execute(this, PlayerState);
	}

	// OnPlayerStateSet에 등록
	return OnPlayerStateSet.Add(Delegate);
}
