#include "CB_GameState.h"
#include "Net/UnrealNetwork.h"

void ACB_GameState::OnRep_CountdownChanged()
{
    // Ŭ�󿡼� UI �ݿ� �뵵 (HUD���� Ÿ�̸� ����)
}

void ACB_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_GameState, bIsCountdownRunning);
}
