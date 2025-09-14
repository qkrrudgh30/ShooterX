// LyraInventoryItemInstance.h

#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "LyraInventoryItemInstance.generated.h"

class ULyraInventoryItemDefinition;
class ULyraInventoryItemFragment;

// 해당 클래스는 인벤토리아이템의 개체로 볼 수 있음.
UCLASS(BlueprintType)
class SHOOTERX_API ULyraInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
	
public:
	UPROPERTY()
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;
	// 인벤토리아이템인스턴스가 어떤 메타정보를 통해 정의되었는지는 인벤토리아이템데피니션이 들고 있음.

};
