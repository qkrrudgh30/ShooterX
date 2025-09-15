// GameUIManagerSubsystem.h

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

class UGameUIPolicy;
class UCommonLocalPlayer;

// GameUIManagerSubsystem 클래스는 GameInstanceSubsystem 클래스를 상속 받음.
// 여기서 주목해야할 점은 UGameUIManagerSubsystem은 UCLASS 속성으로 Abstract 키워드를 가짐.
// 해당 클래스는 단독으로 사용 불가. 이 클래스를 상속 받는 Concrete Class가 필요함.
UCLASS(Abstract, Config=Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameUIManagerSubsystem();

	void SwitchToPolicy(UGameUIPolicy* InPolicy);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	
	virtual void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	
	virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

public:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
	// default UI Policy를 생성할 class
	// 우리는 해당 클래스를 B_LyraGameUIPolicy로 지정할 예정.
	
};
