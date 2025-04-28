// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerInfoWidget.h"
#include "CB_FigtherCharacter.h"
#include "CB_PlayerState.h"

void UCB_PlayerInfoWidget::SetPlayerState(ACB_PlayerState* NewPlayerState)
{
    PlayerState = NewPlayerState;
    if (!PlayerState)
    {
        return;
    }

    if (NameText)
    {
        FString PlayerName = PlayerState->PlayerName;
        NameText->SetText(FText::FromString(PlayerName));
    }

    if (TeamIndexText)
    {
        FString TeamStr = FString::Printf(TEXT("Team %d"), PlayerState->TeamIndex);
        TeamIndexText->SetText(FText::FromString(TeamStr));
    }

    if (HealthProgress)
    {
        ACB_FigtherCharacter* character = Cast<ACB_FigtherCharacter>(PlayerState->GetPawn());
        HealthProgress->SetPercent(character->CurrentHealth / 100);
    }

    if (StockText)
    {
        FString StockStr = FString::Printf(TEXT("x%d"), PlayerState->Lives);
        StockText->SetText(FText::FromString(StockStr));
    }
}
