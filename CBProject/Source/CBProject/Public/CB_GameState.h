#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CB_GameState.generated.h"

UCLASS()
class CBPROJECT_API ACB_GameState : public AGameStateBase
{
	GENERATED_BODY()
	

public:
    UPROPERTY(ReplicatedUsing = OnRep_CountdownChanged)
    bool bIsCountdownRunning = false;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    AActor* SharedCameraActor;

    UFUNCTION()
    void OnRep_CountdownChanged();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void CameraSetting();
};
