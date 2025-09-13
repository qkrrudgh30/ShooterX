// LyraInventoryManagerComponent.cpp

#include "LyraInventoryManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryManagerComponent)

ULyraInventoryManagerComponent::ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}
