// CommonSessionSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

class APlayerController;

// UCommonSession_HostSessionRequest를 만들어서 CommonSessionSubsystem을 전달하면,
// CommonSessionSubsystem에서 MapID와 CmdArgs로 최종적인 CmdArgs를 만들어서 맵 로딩 진행.
UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	FString GetMapName() const;

	FString ConstructTravelURL() const;

public:
	// 준비되어질 MapID(맵 경로)
	UPROPERTY(BlueprintReadWrite, Category = Session)
	FPrimaryAssetId MapID;

	// 전달할 CmdArgs(Experience 이름을 전달함) -> 곧 UserFacingExperienceDefinition에서 보게 될 예정.
	UPROPERTY(BlueprintReadWrite, Category = Session)
	TMap<FString, FString> ExtraArgs;
};

// UCommonUserSessionSubsystem 클래스는 LyraClone에서 사용되어야 하기 때문에 Module Export를 해줘야함.
// 그래서 COMMONUSER_API를 추가해줘야함. 이처럼 다른 모듈에서 사용할 경우 꼭 추가되어야 하는 키워드.
UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCommonSessionSubsystem() {}

	UFUNCTION(BlueprintCallable, Category = "Session")
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);

public:
	// PendingTravelURL은 흔히 맵의 경로로 생각하면 됨.
	FString PendingTravelURL;

};
