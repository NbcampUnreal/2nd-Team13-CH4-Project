#include "CB_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void ACB_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_GameState, bIsCountdownRunning);
    DOREPLIFETIME(ACB_GameState, SharedCameraActor);
}

void ACB_GameState::OnRep_SharedCameraActor()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if(PC && PC->IsLocalController() && SharedCameraActor)
        {
            PC->SetViewTargetWithBlend(SharedCameraActor, 0.5f);
        }
    }
}

void ACB_GameState::OnRep_CountdownChanged()
{
    // 클라에서 UI 반영 용도 (HUD에서 타이머 시작)
}
