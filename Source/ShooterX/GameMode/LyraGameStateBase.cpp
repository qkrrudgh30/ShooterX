// LyraGameStateBase.cpp


#include "GameMode/LyraGameStateBase.h"
#include "LyraExperienceManagerComponent.h"

ALyraGameStateBase::ALyraGameStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<ULyraExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
