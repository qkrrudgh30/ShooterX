// UIExtensionSubsystem.cpp

#include "UIExtensionSubsystem.h"
#include "Blueprint/UserWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(UIExtensionSubsystem)

void FUIExtensionHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtension(*this);
	}
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	// ExtensionPointTag(Slot)이 Invalid
	if (false == ExtensionPointTag.IsValid())
	{
		return FUIExtensionHandle();
	}

	// WidgetClass가 없으면
	if (nullptr == Data)
	{
		return FUIExtensionHandle();
	}

	// ExtensionPointTag를 기반하여 ExtensionList를 추출
	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	// 새로운 UIExtension을 추가 진행
	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef(MakeShared<FUIExtension>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	return FUIExtensionHandle(this, Entry);
}

void UUIExtensionSubsystem::UnregisterExtension(const FUIExtensionHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		// 반드시 해당 ExtensionHandle이 UUIExtensionSubsystem과 같은지 확인 해야함.
		checkf(ExtensionHandle.ExtensionSource == this, TEXT("Trying to unregister an extension that's not from this extension subsystem."));

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;

		// Extension의 PointTag를 통해 ExtensionMap에서 해당 Slot에 있는지 찾아서 제거.
		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			ListPtr->RemoveSwap(Extension);
			if (ListPtr->Num() == 0)
			{
				// Num() == 0이면 Map에서도 제거 진행
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
	}
}
