#include "CB_PlayerState.h"
#include "Net/UnrealNetwork.h"

ACB_PlayerState::ACB_PlayerState()
{
    bReplicates = true;
}

void ACB_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_PlayerState, Lives);
    DOREPLIFETIME(ACB_PlayerState, TeamIndex);
}