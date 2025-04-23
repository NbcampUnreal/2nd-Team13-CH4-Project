#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DynamicCameraActor.h"
#include "CB_GameMode.generated.h"

UCLASS()
class CBPROJECT_API ACB_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACB_GameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 CountdownTime;
	void StartPlay();
	void TickCountdown();
	void StartGame();
	void UpdateCountdownUI();
	void RespawnPlayer(AController* Controller);

	void HandlePlayerDeath(AController* DeadController);
	void HandleRespawn(AController* Controller);

	FTimerHandle CountdownTimerHandle;

	UPROPERTY(ReplicatedUsing=OnRep_CountdownChanged)
	bool bIsCountdownRunning = false;

	UFUNCTION()
	void OnRep_CountdownChanged();

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<ADynamicCameraActor> CameraActorClass;

	void SetTeam(AController* NewPlayer);


protected:
	TArray<AActor*> SpawnPoints;
};
