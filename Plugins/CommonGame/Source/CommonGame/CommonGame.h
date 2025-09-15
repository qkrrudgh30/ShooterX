// CommonGame.h

#pragma once

#include "Modules/ModuleManager.h"

class FCommonGameModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

};
