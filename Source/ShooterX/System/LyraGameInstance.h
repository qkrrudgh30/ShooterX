// LyraGameInstance.h

#pragma once

#include "CommonGame/CommonGameInstance.h"
#include "LyraGameInstance.generated.h"

/**
 * GameInstance는 게임 프로세스(.exe)에서 하나만 존재하는 개체로 생각하면 됨.(== High-Level Manager Object)
 * - 게임이 켜질 때 만들어지고, 꺼지기 전까지 살아있음.
 * - Editor 상에서는 PIE로 실행 될 때마다 하나씩 생성됨. 즉, 에디터에서는 복수 개의 GameInstance가 존재 가능. (ex. 클라1, 클라2, 서버, ....) 
 */
UCLASS()
class SHOOTERX_API ULyraGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:
	ULyraGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init() override;

	virtual void Shutdown() override;

};
