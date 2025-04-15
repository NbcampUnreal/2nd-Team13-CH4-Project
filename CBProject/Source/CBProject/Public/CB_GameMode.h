
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CB_GameMode.generated.h"

UCLASS()
class CBPROJECT_API ACB_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void StartPlay();
	void StartGame();

	void HandleRespawn(AController* Controller);

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(ReplicatedUsing=OnRep_CountdownChanged)
	bool bIsCountdownRunning = false;

	UFUNCTION()
	void OnRep_CountdownChanged();

protected:
	TArray<AActor*> SpawnPoints;
};
