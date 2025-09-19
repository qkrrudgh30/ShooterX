// LyraReticleWidgetBase.cpp

#include "LyraReticleWidgetBase.h"
#include "Equipment/Weapon/LyraWeaponInstance.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraReticleWidgetBase)

ULyraReticleWidgetBase::ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraReticleWidgetBase::InitializeFromWeapon(ULyraWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;

	if (WeaponInstance)
	{
		InventoryInstance = Cast<ULyraInventoryItemInstance>(WeaponInstance->GetInstigator());
	}
}