// LyraControllerComponent_CharacterParts.h

#pragma once

#include "Components/ControllerComponent.h"
#include "LyraCharacterPartTypes.h"
#include "LyraControllerComponent_CharacterParts.generated.h"

class ULyraPawnComponent_CharacterParts;

// ControllerComponent가 소유하는 캐릭터파츠.
USTRUCT()
struct FLyraControllerCharacterPartEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FLyraCharacterPart Part;
	// 캐릭터파츠에 대한 정의(메타 데이터)
	// 당연히 장착 전부터 데이터가 채워져있어야 장착이 가능.

	FLyraCharacterPartHandle Handle;
	// 캐릭터파츠핸들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한(인스턴스) 캐릭터파츠 핸들값
	// 다시 말하면, 장착된 뒤에 반환되는 핸들값을 저장하는 변수.

};

// Controller가 Pawn을 Possess 했을 때, Pawn에 어떤 Cosmetic Actor를 생성할 지 결정하는 ControllerComponent.
// - 필자는 캐릭터 파츠를 유저 관점에서 관리하는 Component로 이해.
UCLASS(meta=(BlueprintSpawnableComponent))
class ULyraControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()

public:
	ULyraControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category=Cosmetic)
	void AddCharacterPart(const FLyraCharacterPart& NewPart);

	void AddCharacterPartInternal(const FLyraCharacterPart& NewPart);
	// -Internal이 붙으면 블루프린트가 아니라 Unreal C++로직이 작성된 함수라는 뜻.
		
	ULyraPawnComponent_CharacterParts* GetPawnCustomizer() const;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void RemoveAllCharacterParts();

public:
	UPROPERTY(EditAnywhere, Category=Cosmetic)
	TArray<FLyraControllerCharacterPartEntry> CharacterParts;

};
