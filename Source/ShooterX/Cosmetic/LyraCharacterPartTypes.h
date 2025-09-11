// LyraCharacterPartTypes.h

#pragma once

#include "GameFramework/Actor.h"
#include "LyraCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FLyraCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

public:
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

};

USTRUCT(BlueprintType)
struct FLyraCharacterPart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;
	// 파츠로 생성된 Actor의 Class 정보를 가지고 있음. -> Actor BP를 하나의 파츠로 생각하면 됨.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	// Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보(따로 정의되어 있지 않으면 그냥 Root에 붙힘)

};
