// LyraCommonActivatableWidget.cpp

#include "LyraCommonActivatableWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCommonActivatableWidget)

ULyraCommonActivatableWidget::ULyraCommonActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> ULyraCommonActivatableWidget::GetDesiredInputConfig() const
{
	// 앞서 정의 했던 WidgetInputMode에 따라 InputConfig를 설정해줘야함.
	// - ECommonInputMode에 따라 Input의 흐름을 Game/Menu 혹은 둘 다  정하는것이 가능함.
	// - 예로들어 우리가 Menu로 들어갔을 경우 더이상 Game에 Input을 보내고 싶지 않을 경우 매우 용이.
	// -- 상상해보자. Menu 모드인데 Space를 누르거나 MouseClick으로 총알이 나간다면.

	switch (InputConfig)
	{
	case ELyraWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case ELyraWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case ELyraWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCaptureMode);
	case ELyraWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}

}
