// LyraPlayerController.cpp


#include "Player/LyraPlayerController.h"
#include "Camera/LyraPlayerCameraManager.h"

ALyraPlayerController::ALyraPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraPlayerCameraManager::StaticClass();
}
