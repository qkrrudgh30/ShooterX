// GFA_AddGameplayCuePath.cpp

#include "GFA_AddGameplayCuePath.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GFA_AddGameplayCuePath)

UGFA_AddGameplayCuePath::UGFA_AddGameplayCuePath()
	: Super()
{
	DirectoryPathsToAdd.Add(FDirectoryPath { TEXT("/GameplayCue") });
}
