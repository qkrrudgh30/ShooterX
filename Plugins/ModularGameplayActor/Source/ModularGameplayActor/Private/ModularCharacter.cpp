// ModularCharacter.cpp

#include "ModularCharacter.h"
#include "Components/GameFrameworkComponentManager.h"

void AModularCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularCharacter::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	// LCHeroComponent::InitializePlayerInput() 함수에서도 호출했던 함수.

	Super::BeginPlay();
}

void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	// 순서가 중요함. Super::EndPlay()보다 전에 호출되어야함.

	Super::EndPlay(EndPlayReason);
}
