// LyraGameplayAbility_FromEquipment.cpp

#include "LyraGameplayAbility_FromEquipment.h"
#include "Equipment/LyraEquipmentInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayAbility_FromEquipment)

ULyraGameplayAbility_FromEquipment::ULyraGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULyraEquipmentInstance* ULyraGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	// CurrentActorInfo의 AbilitySystemComponent와 CurrentSpecHandle을 활용하여 GameplayAbilitySpec을 가져옴.
	// - CurrentSpecHandle은 SetCurrentActorInfo() 함수를 호출할 때 핸들 값을 받아서 저장됨.
	// - CurrentSpecHandle과 CurrentActorInfo는 같이 저장됨.
	// - FindAbilitySpecFromHandle() 함수는 GiveAbility() 함수로 부여된 ActivatableAbilities를 순회하며 GameplayAbilitySpec을 찾아냄.
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// GameplayAbility_FromEquipment는 EquipmentInstance로부터 GiveAbility() 함수를 진행 했으므로 SourceObject에 EquipmentInstanc가 저장되어 잇음.
		return Cast<ULyraEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}
