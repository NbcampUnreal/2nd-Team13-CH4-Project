// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_CharacterSelect.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "CB_PlayerState.h"
#include "CB_PlayerSelectCharacterWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

void UCB_CharacterSelect::NativeConstruct()
{
	Super::NativeConstruct();

	CreatePlayerSlots();

}

void UCB_CharacterSelect::CreatePlayerSlots()
{
    UE_LOG(LogTemp, Display, TEXT("Select - CreatePlayerSlots"));

    if (!VerticalBox_ClientList)
    {
        return;
    }

    VerticalBox_ClientList->ClearChildren();

    AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
    if (!GameState)
    {
        UE_LOG(LogTemp, Warning, TEXT("Select - GameState is nullptr"));
        return;
    }

    int32 PlayerCount = GameState->PlayerArray.Num();
    UE_LOG(LogTemp, Display, TEXT("Select - PlayerArray Num: %d"), PlayerCount);

    for (APlayerState* PS : GameState->PlayerArray)
    {
        ACB_PlayerState* CBPS = Cast<ACB_PlayerState>(PS);
        if (CBPS)
        {
            UCB_PlayerSelectCharacterWidget* PlayerSelectCharacterWidget = CreateWidget<UCB_PlayerSelectCharacterWidget>(GetWorld(), PlayerSelectCharacterWidgetClass);
            if (PlayerSelectCharacterWidget)
            {
                PlayerSelectCharacterWidget->SetPlayerTeam(CBPS);
                PlayerSelectCharacterWidget->SetPlayerName(CBPS);

                VerticalBox_ClientList->AddChild(PlayerSelectCharacterWidget);
                UE_LOG(LogTemp, Display, TEXT("Select - AddChild: %s"), *CBPS->PlayerName);
            }
        }
    }
}


void UCB_CharacterSelect::UpdatePlayerTeam()
{
	if (VerticalBox_ClientList)
	{
		AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
		if (!GameState)
		{
			return;
		}

		int32 Index = 0;
		for (APlayerState* PS : GameState->PlayerArray)
		{
			ACB_PlayerState* CBPS = Cast<ACB_PlayerState>(PS);
			if (CBPS)
			{
				UCB_PlayerSelectCharacterWidget* PlayerSelectCharacterWidget = Cast<UCB_PlayerSelectCharacterWidget>(VerticalBox_ClientList->GetChildAt(Index));
				if (PlayerSelectCharacterWidget)
				{
					PlayerSelectCharacterWidget->SetPlayerTeam(CBPS);
				}
			}
			++Index;
		}
	}
}

void UCB_CharacterSelect::SetStartButtonEnabled()
{
}

void UCB_CharacterSelect::SetIsTeam()
{
}
