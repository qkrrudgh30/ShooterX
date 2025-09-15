// CommonGameInstance.cpp

#include "CommonGameInstance.h"
#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonGameInstance)

UCommonGameInstance::UCommonGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnValue = Super::AddLocalPlayer(NewPlayer, UserId);
	// GameInstance에 관리하는 Player 컨테이너에 추가. Return Value는 참고로 새로 추가된 Index.
	if (INDEX_NONE != ReturnValue)
	{
		// PrimaryPlayer는 처음만 캐싱하는 듯 함. 무조건 처음 세팅되면 메인인가 싶음. Index == 0을 메인으로 잡긴함.
		if (false == PrimaryPlayer.IsValid())
		{
			PrimaryPlayer = NewPlayer;
		}

		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
		// GameUIManagerSubsystem을 통해 NotifyPlayerAdded() 함수 호출로 GameLayout을 추가함.
	}

	return ReturnValue;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		PrimaryPlayer.Reset();
	}

	GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));
	return Super::RemoveLocalPlayer(ExistingPlayer);
}
