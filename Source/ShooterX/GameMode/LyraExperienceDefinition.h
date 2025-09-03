// LyraExperienceDefinition.h

#pragma once

#include "Engine/DataAsset.h"
#include "LyraExperienceDefinition.generated.h"

class ULyraPawnData;

UCLASS(BlueprintType)
class SHOOTERX_API ULyraExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULyraPawnData> DefaultPawnData;
	// DefaultPawnData에는 Pawn Class, InputMapping, ... 정보가 들어갈 예정.

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
	// 게임 모드에 따른 GameFeature 플러그인을 로딩하는데 이에 대한 연결고리 같은 속성.
	// 후일 ShooterCore 관련 플러그인 작업을 할 경우 진행하게 될 예정.

};