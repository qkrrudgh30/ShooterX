// UIExtensionSystem.h

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Templates/SharedPointer.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.generated.h"

UENUM(BlueprintType)
enum class EUIExtensionAction : uint8
{
	Added,
	Removed,
};

// ExtensionPoint GameplayTag 비교 방법
UENUM(BlueprintType)
enum class EUIExtensionPointMatch : uint8
{
	// A.B는 A.B.C를 허용하지 않는다.
	ExactMatch,

	// A.B는 A.B.C를 허용한다.
	PartialMatch,
};

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

// 프로그래머가 UIExtensionPoint의 태그와 AllowedDataClasses에 맞게 UIExtension 위젯을 만들어옴.
// 그럼 해당 UIExtension 위젯을 UIExtensionRequest를 통해서 UIExtensionPointWidget에 장착해달라는 요청을 보낼때 사용함.
USTRUCT(BlueprintType)
struct FUIExtensionRequest
{
	GENERATED_BODY()

public:
	// UIExtensionPoint와 연동될 Extension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIExtensionHandle ExtensionHandle;

	// Extension의 Slot GameplayTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ExtensionPointTag;

	// WidgetClass로 FUIExtension과 같다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data = nullptr;

	// 누가 요청했는가에 대한 정보. FUIExtension의 ContextObject로 전달받음. (UCommonLocalPlayer, UModularPlayerState, ... == UIExtension을 실행한 Instingator)
	// 등록은 LocalPlayer가 했는데, 해제는 PlayerState가 시킨다면 해당 해제 요청은 드랍되어야하기 때문에 이 속성을 가지고 판별함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;

};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EUIExtensionAction Action, const FUIExtensionRequest& Request);

// UIExtensionPointWidget을 만들긴 했지만, 직접적으로 해당 위젯을 참조할 수 없게 할 예정.
// UIExtensionPoint을 만들고 UIExtensionSubsystem에서 관리하게끔 함.
// 즉, 프로그래머는 UIExtensionSubsystem에 GameplayTag를 넘겨주면서 UIExtensionPoint를 얻어옴.
// 그럼 UIExtensionPoint의 태그와 AllowedDataClasses를 체크 후 델리게이트를 통해 프로그래머가 만든 UIExtension이 UIExtensionPointWidget에 장착되게끔 할 예정.
struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{
public:
	bool DoesExtensionPassContract(const struct FUIExtension* Extension) const;
	
public:
	// UIExtension의 Slot GameplayTag
	FGameplayTag ExtensionPointTag;

	// UIExtension을 생성/제거한 Instingator(주체)
	TWeakObjectPtr<UObject> ContextObject;

	// UIExtensionPointWidget에 혀용된 WidgetClass -> UIExtensionPointWidget's DataClasses
	TArray<UClass*> AllowedDataClasses;

	// Widget을 ExtensionPointWidget에 연결하기 위한 Callback 함수
	FExtendExtensionPointDelegate Callback;

	EUIExtensionPointMatch ExtensionPointTagMatchType = EUIExtensionPointMatch::ExactMatch;

};

// 프로그래머 -> UIExtensionPointHandle -> UIExtensionPoint -> UIExtensionPointWidget의 흐름.
USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionPointHandle
{
	GENERATED_BODY()

public:
	FUIExtensionPointHandle() {}

	FUIExtensionPointHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtensionPoint>& InDataPtr)
		: ExtensionSource(InExtensionSource)
		, DataPtr(InDataPtr)
	{

	}
	
	bool operator==(const FUIExtensionPointHandle& Other) const { return DataPtr == Other.DataPtr; }
	
	bool operator!=(const FUIExtensionPointHandle& Other) const { return !operator==(Other); }
	
	bool IsValid() const { return DataPtr.IsValid(); }

	friend uint32 GetTypeHash(const FUIExtensionPointHandle& Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

	void Unregister();

public:
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;

	TSharedPtr<FUIExtensionPoint> DataPtr;

};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);

	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);

	void UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle);

	FUIExtensionPointHandle RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	FUIExtensionPointHandle RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	// UIExtension과 UIExtensionPoint 관리 싱크를 위한 메서드 추가.
	// ExtensionPoint --(Broadcast)--> Extensions [ExtensionPoint가 추가/제거 되었을 경우, Extension에 알림: 참고로 Added만함]
	void NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint);

	// Extension --(Broadcast)-->ExtensionPoints [Extension이 추가/제거 되었을 경우, Extension Points에 알림]
	void NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension);

	FUIExtensionRequest CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension);
	
public:
	// GameplayTag(Slot) --- FUIExtension(WidgetClass)
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;

	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;
	// GameplayTag(Slot) --- FUIExtensionPoint(WidgetClassWithContext)	
	
};
