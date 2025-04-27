#include "CB_PlayerState.h"
#include "Net/UnrealNetwork.h"

ACB_PlayerState::ACB_PlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
	Lives = 3;
	CharacterIndex = 0;
	TeamIndex = 0;
    bReplicates = true;
}

void ACB_PlayerState::SetPlayerLives(int32 NewLives)
{
    Lives = NewLives;
}

void ACB_PlayerState::SetPlayerCharacterIndex(int32 NewCharacterIndex)
{
    CharacterIndex = NewCharacterIndex;
}

void ACB_PlayerState::SetPlayerName(FString NewPlayerName)
{
	PlayerName = NewPlayerName;
}

void ACB_PlayerState::SetPlayerTeam(int32 NewTeamIndex)
{
    TeamIndex = NewTeamIndex;
}

void ACB_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_PlayerState, Lives);
    DOREPLIFETIME(ACB_PlayerState, CharacterIndex);
    DOREPLIFETIME(ACB_PlayerState, TeamIndex);
}