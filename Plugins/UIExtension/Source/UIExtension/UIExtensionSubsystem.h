// UIExtensionSubsystem.h

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Templates/SharedPointer.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSubsystem.generated.h"

struct FUIExtension : TSharedFromThis<FUIExtension>
{
	FGameplayTag ExtensionPointTag;

	UObject* Data = nullptr;
	// 위젯의 클래스 정보를 가지고 있으며
	// UUIExtensionSubsystem에서 AddReferencedObjects에서 GC를 막음.

	TWeakObjectPtr<UObject> ContextObject;
	// 보통 LocalPlayer로 설정됨.

	int32 Priority = INDEX_NONE;
	
};

USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()

	friend class UUIExtensionSubsystem;

public:
	FUIExtensionHandle() 
	{
	}

	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtension>& InDataPtr)
		: ExtensionSource(InExtensionSource)
		, DataPtr(InDataPtr)
	{

	}

	bool operator==(const FUIExtensionHandle& Other) const { return DataPtr == Other.DataPtr; }

	bool operator!=(const FUIExtensionHandle& Other) const { return !operator==(Other); }

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }
	
	friend FORCEINLINE uint32 GetTypeHash(FUIExtensionHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}
	
public:
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;

	TSharedPtr<FUIExtension> DataPtr;

};

template<>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);

	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);

public:
	// GameplayTag(Slot) --- FUIExtension(WidgetClass)
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;
	
};
