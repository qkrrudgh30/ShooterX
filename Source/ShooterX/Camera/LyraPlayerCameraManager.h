// LyraPlayerCameraManager.h

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "LyraPlayerCameraManager.generated.h"

#define LYRA_CAMERA_DEFAULT_FOV (80.f)
#define LYRA_CAMERA_DEFAULT_PITCH_MIN (-89.f)
#define LYRA_CAMERA_DEFAULT_PITCH_MAX (89.f)

UCLASS()
class SHOOTERX_API ALyraPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ALyraPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
