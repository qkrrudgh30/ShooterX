// UIExtensionModule.cpp

#include "UIExtensionModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

class FUIExtensionModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FUIExtensionModule::StartupModule()
{

}

void FUIExtensionModule::ShutdownModule()
{

}

IMPLEMENT_MODULE(FUIExtensionModule, UIExtension);