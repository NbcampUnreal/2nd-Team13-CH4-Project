#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CB_GameState.generated.h"

UCLASS()
class CBPROJECT_API ACB_GameState : public AGameStateBase
{
	GENERATED_BODY()
	

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(ReplicatedUsing = OnRep_SharedCameraActor, EditAnywhere, BlueprintReadOnly, Category = "Camera")
    AActor* SharedCameraActor = nullptr;

    UFUNCTION()
    void OnRep_SharedCameraActor();

    UFUNCTION()
    void OnRep_CountdownChanged();

    UPROPERTY(ReplicatedUsing = OnRep_CountdownChanged)
    bool bIsCountdownRunning = false;
};
