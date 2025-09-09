// LyraUserFacingExperienceDefinition.cpp

#include "LyraUserFacingExperienceDefinition.h"
#include "CommonUser/CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* ULyraUserFacingExperienceDefinition::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	// 고찰이 필요한 내용.
	// - UCommonSession_HostSessionRequest는 UObject로 생성 해놓고 알아서 GC가 됨.
	// - 해당 개체는 현재 프레임에서 사용하기 때문에, GC에 대한 염려가 없음.
	// - 만약 다음 프레임이든 추가적인 프레임 상에서 해당 개체를 사용할 경우, Lifetime 관리 필요.
	// - 그렇지 않으면 Dangling Pointer 됨.
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
