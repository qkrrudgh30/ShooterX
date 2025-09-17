// LyraCommonActivatableWidget.h

#pragma once

#include "CommonActivatableWidget.h"
#include "LyraCommonActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ELyraWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

// CommonActivatableWidget의 주석을 보면 복잡하게 설명되어 있음.
// 필자가 이해한 CommonActivatableWidget은 두 가지 특성을 가지고 있음.
// 1. Widget Layout과 Widget Instance를 구분하기 위한 용도로 CommonActivatableWidget은 Layout 정의.
//   - 런타임 Activate/Deactivate
//   - WidgetTree에서 제거가 아닌 껏다 / 켰다 (== Activate / Deactivate)
// 2. Input(Mouse or Keyboard etc ...) 처리 방법 정의
UCLASS(Abstract, Blueprintable)
class SHOOTERX_API ULyraCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	ULyraCommonActivatableWidget(const FObjectInitializer& ObjectInitializer);

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

public:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	ELyraWidgetInputMode InputConfig = ELyraWidgetInputMode::Default;
	// Input 처리 방식

	UPROPERTY(EditDefaultsOnly, Category=Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	// Mouse 처리 방식

};
