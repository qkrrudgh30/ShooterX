// LyraCircumferenceMarkerWidget.cpp

#include "SCircumferenceMarkerWidget.h"
#include "Engine/UserInterfaceSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(SCircumferenceMarkerWidget)

SCircumferenceMarkerWidget::SCircumferenceMarkerWidget()
{
}

void SCircumferenceMarkerWidget::Construct(const FArguments& InArgs)
{
	MarkerBrush = InArgs._MarkerBrush;
	MarkerList = InArgs._MarkerList;
	Radius = InArgs._Radius;
	bColorAndOpacitySet = InArgs._ColorAndOpacity.IsSet();
	ColorAndOpacity = InArgs._ColorAndOpacity;
}

FSlateRenderTransform SCircumferenceMarkerWidget::GetMarkerRenderTransform(const FCircumferenceMarkerEntry& Marker,
                                                                             const float BaseRadius, const float HUDScale) const
{
	float XRadius = BaseRadius;
	float YRadius = BaseRadius;
	if (bReticleCornerOutsideSpreadRadius)
	{
		// Image 사이즈의 반만큼 (Radius) 더해줘서 원 바깥에서 마커가 랜더링 되도록 함.
		XRadius += MarkerBrush->ImageSize.X * 0.5f;
		YRadius += MarkerBrush->ImageSize.Y * 0.5f;
	}

	// Degree -> Radians
	const float LocalRotationRadians = FMath::DegreesToRadians(Marker.ImageRotationAngle);
	const float PositionAngleRadians = FMath::DegreesToRadians(Marker.PositionAngle);

	FSlateRenderTransform RotationAboutOrigin(
		// RotationAboutOrigin은 ImageSize/2.0f 만큼 옮기고 회전시키고 다시 옮겨놓는다.
		Concatenate(
			FVector2D(-MarkerBrush->ImageSize.X * 0.5f, -MarkerBrush->ImageSize.Y * 0.5f),
			FQuat2D(LocalRotationRadians),
			FVector2D(MarkerBrush->ImageSize.X * 0.5f, MarkerBrush->ImageSize.Y * 0.5f)			
		)	
	);

	// 회전한 image를 circumference 방식으로 위치를 translation 시켜줌.
	return TransformCast<FSlateRenderTransform>(
		Concatenate(
			RotationAboutOrigin,
			FVector2D(XRadius * FMath::Sin(PositionAngleRadians) * HUDScale, -YRadius * FMath::Cos(PositionAngleRadians) * HUDScale)
		)
	);
}

int32 SCircumferenceMarkerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	// GetLocalPositionAtCoordinate(2D(0.5, 0.5)) == 중심좌표
	const FVector2D LocalCenter = AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f));

	const bool bDrawMarkers = (0 < MarkerList.Num()) && (MarkerBrush != nullptr);
	if (bDrawMarkers)
	{
		const FLinearColor MarkerColor = bColorAndOpacitySet ?
			ColorAndOpacity.Get().GetColor(InWidgetStyle) :
			(InWidgetStyle.GetColorAndOpacityTint() * MarkerBrush->GetTint(InWidgetStyle));

		// 아래의 공식은 숙제로 한 번 유도해보자.
		if (KINDA_SMALL_NUMBER < MarkerColor.A)
		{
			const float BaseRadius = Radius.Get();
			const float ApplicationScale = GetDefault<UUserInterfaceSettings>()->ApplicationScale;
			for (const FCircumferenceMarkerEntry& Marker : MarkerList)
			{
				const FSlateRenderTransform MarkerTransform = GetMarkerRenderTransform(Marker, BaseRadius, ApplicationScale);
				const FPaintGeometry Geometry(
					AllottedGeometry.ToPaintGeometry(MarkerBrush->ImageSize, FSlateLayoutTransform(LocalCenter - (MarkerBrush->ImageSize * 0.5f)),
						MarkerTransform,
						FVector2D(0.f, 0.f)
					)
				);
				FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geometry, MarkerBrush, DrawEffects, MarkerColor);
			}
		}
	}

	return LayerId;
}

FVector2D SCircumferenceMarkerWidget::ComputeDesiredSize(float) const
{
	// ComputeDesiredSize()는 Immediate 구조 UI 프레임워크에서는 매프레임 호출됨.
	// - Retained Vs. Immediate
	check(MarkerBrush);
	const float SampledRadius = Radius.Get();
	return FVector2D((MarkerBrush->ImageSize.X + SampledRadius) * 2.f, (MarkerBrush->ImageSize.Y + SampledRadius) * 2.f);
}

void SCircumferenceMarkerWidget::SetRadius(float NewRadius)
{
	if (Radius.IsBound() || (Radius.Get()) != NewRadius)
	{
		Radius = NewRadius;
		Invalidate(EInvalidateWidgetReason::Layout);
	}
}

void SCircumferenceMarkerWidget::SetMarkerList(TArray<FCircumferenceMarkerEntry>& NewMarkerList)
{
	MarkerList = NewMarkerList;
}
