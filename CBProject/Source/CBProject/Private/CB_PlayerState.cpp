#include "CB_PlayerState.h"
#include "Net/UnrealNetwork.h"

void ACB_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_PlayerState, Lives);
}