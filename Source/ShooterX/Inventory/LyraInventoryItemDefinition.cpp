// LyraInventoryItemDefinition.cpp

#include "LyraInventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryItemDefinition)

ULyraInventoryItemDefinition::ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const ULyraInventoryItemFragment* ULyraInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		// Fragments를 순회하며 IsA()를 통해 해당 클래스를 가지고 있는지 확인.
		for (ULyraInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
