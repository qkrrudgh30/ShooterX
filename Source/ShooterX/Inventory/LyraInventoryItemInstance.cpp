// LyraInventoryItemInstance.cpp

#include "LyraInventoryItemInstance.h"
#include "LyraInventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryItemInstance)

ULyraInventoryItemInstance::ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const ULyraInventoryItemFragment* ULyraInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		// LyraInventoryItemDefinition은 모든 멤버 변수가 EditDefaultsOnly로 선언되어 있으므로, GetDefault() 함수로 가져와도 무관.
		// - Fragment 정보는 Instance가 아닌 Definition에 있음.
		return GetDefault<ULyraInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
