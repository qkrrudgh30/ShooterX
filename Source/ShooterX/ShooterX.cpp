// ShooterX.cpp

#include "ShooterX.h"
#include "Modules/ModuleManager.h"
#include "System/LyraAssetManager.h"

DEFINE_LOG_CATEGORY(LogSX);
DEFINE_LOG_CATEGORY(LogSXNet);

class FSXModule : public FDefaultGameModuleImpl
{
	typedef FDefaultGameModuleImpl Super;

public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

};

void FSXModule::StartupModule()
{
	Super::StartupModule();
    
	UE_LOG(LogSX, Log, TEXT("FSXModule::StartupModule()"));

	ULyraAssetManager::ShouldLogAssetLoads();
}

void FSXModule::ShutdownModule()
{
	UE_LOG(LogSX, Log, TEXT("FSXModule::ShutdownModule()"));
    
	Super::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FSXModule, ShooterX, "ShooterX" );
