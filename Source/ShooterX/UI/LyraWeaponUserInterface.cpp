// LyraWeaponUserInterface.cpp

#include "LyraWeaponUserInterface.h"
#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Equipment/Weapon/LyraWeaponInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraWeaponUserInterface)

ULyraWeaponUserInterface::ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pawn을 가져오고
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		// EquipmentManagetComponent를 활용하여 WeaponInstance를 가져오자
		if (ULyraEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>())
		{
			if (ULyraWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<ULyraWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					// 새로 업데이트 해주고 OnWeaponChanged 호출 진행
					ULyraWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}
