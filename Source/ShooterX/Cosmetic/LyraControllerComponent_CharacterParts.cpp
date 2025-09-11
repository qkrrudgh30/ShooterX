// LyraControllerComponent_CharacterParts.cpp

#include "LyraControllerComponent_CharacterParts.h"
#include "LyraPawnComponent_CharacterParts.h"

ULyraControllerComponent_CharacterParts::ULyraControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraControllerComponent_CharacterParts::AddCharacterPart(const FLyraCharacterPart& NewPart)
{
	// 여기까지만 봤을 때 헷갈리면 안되는 것이 ControllerComponent_CharacterPart에서는 파츠를 아직 장착하진 못한거임. 들고만 있음.

	AddCharacterPartInternal(NewPart);
}

void ULyraControllerComponent_CharacterParts::AddCharacterPartInternal(const FLyraCharacterPart& NewPart)
{
	FLyraControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	// AddDefaulted_GetRef() 함수의 동작원리도 확인해보면 좋음.
	// 원래라면 파츠앤트리 개체를 만들고 CharacterParts에 추가하는 식인데, 그렇게하면 복사 연산이 일어나게됨.
	// AddDefaulted_GetRef()는 내부적으로 공간을 마련하고 그 공간의 주소를 넘겨줌. 따라서 복사는 포인터 크기만큼만 이뤄지는 것.
	NewEntry.Part = NewPart;

	if (ULyraPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
		// PawnComponent_CharacterPart의 AddCharacterPart()는 진짜 장착이 시작됨. 스폰됨.
		// 원래 라이라에서는 ControllerComponent_CharacterPart::AddCharacterPart() 함수는 장착시킬 파츠를 추가하는 함수이고
		// PawnComponent_CharacterPart::AddCharacterPart() 함수가 진짜로 장착시키는 함수임. 둘이 나눠져 있고 지금처럼 곧바로 호출되지는 않음.
	}
}

ULyraPawnComponent_CharacterParts* ULyraControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면 우리가 앞서 LyraPawnComponent_CharacterParts를 상속받는 B_MannequinPawnCosmetics를 B_Hero_ShooterMannequin에 추가함.
		// B_MannequinPawnComponent가 반환되길 기대함.
		return ControlledPawn->FindComponentByClass<ULyraPawnComponent_CharacterParts>();
	}

	return nullptr;
}

void ULyraControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (true == HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void ULyraControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();

	Super::EndPlay(EndPlayReason);
}

void ULyraControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 캐릭터파츠를 전부 제거
	if (ULyraPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<ULyraPawnComponent_CharacterParts>() : nullptr)
	{
		for (FLyraControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 캐릭터파츠를 추가
	if (ULyraPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<ULyraPawnComponent_CharacterParts>() : nullptr)
	{
		for (FLyraControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}

void ULyraControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (ULyraPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FLyraControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}

	CharacterParts.Reset();
}
