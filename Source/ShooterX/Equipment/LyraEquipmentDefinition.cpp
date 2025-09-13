// LyraEquipmentDefinition.cpp

#include "LyraEquipmentDefinition.h"
#include "LyraEquipmentInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraEquipmentDefinition)

ULyraEquipmentDefinition::ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = ULyraEquipmentInstance::StaticClass();
	// 기본값으로 LyraEquipmentInstance가 지정됨.
}
