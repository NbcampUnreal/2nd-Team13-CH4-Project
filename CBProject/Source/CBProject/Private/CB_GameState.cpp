#include "CB_GameState.h"
#include "Net/UnrealNetwork.h"

void ACB_GameState::OnRep_CountdownChanged()
{
    // 클라에서 UI 반영 용도 (HUD에서 타이머 시작)
}

void ACB_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_GameState, bIsCountdownRunning);
    DOREPLIFETIME(ACB_GameState, SharedCameraActor);
}

void ACB_GameState::CameraSetting()
{
    if (GetWorld())
    {
        SharedCameraActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    }
}