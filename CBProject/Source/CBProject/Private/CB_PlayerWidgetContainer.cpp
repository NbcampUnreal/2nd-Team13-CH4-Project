// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerWidgetContainer.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"
#include "CB_GameState.h"
#include "Kismet/GameplayStatics.h"

void UCB_PlayerWidgetContainer::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCB_PlayerWidgetContainer::UpdatePlayerList()
{
	if (PlayerInfoContainer)
	{
		/*
		PlayerInfoContainer->ClearChildren();

		APlayerController* LocalController = GetOwningPlayer();
		if (LocalController)
		{
			ACB_PlayerState* LocalPlayerState = Cast<ACB_PlayerState>(LocalController->PlayerState);
			if (LocalPlayerState)
			{
				UCB_PlayerInfoWidget* PlayerInfoWidget = CreateWidget<UCB_PlayerInfoWidget>(GetWorld(), UCB_PlayerInfoWidget::StaticClass());

				if (PlayerInfoWidget)
				{
					PlayerInfoWidget->SetPlayerState(LocalPlayerState);
					PlayerInfoContainer->AddChild(PlayerInfoWidget);
				}
			}
		}
		*//*
        AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
        if (!GameState)
        {
            return;
        }*/
		UWorld* World = GetWorld();
		if (!World)
		{
			UE_LOG(LogTemp, Warning, TEXT("World is null"));
			return;
		}

		AGameStateBase* GameState = Cast<ACB_GameState>(UGameplayStatics::GetGameState(World));
		if (!GameState)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameState is null"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("PlayerArray Num: %d"), GameState->PlayerArray.Num());

		if (GameState->PlayerArray.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerArray is empty"));
			return;
		}

		for (APlayerState* PS : GameState->PlayerArray)
        {

            ACB_PlayerState* CB_PS = Cast<ACB_PlayerState>(PS);
            if (!CB_PS)
            {
                continue;
            }

            if (PlayerInfoItemClass)
            {
                UCB_PlayerInfoWidget* PlayerInfoItem = CreateWidget<UCB_PlayerInfoWidget>(GetWorld(), PlayerInfoItemClass);
                if (PlayerInfoItem)
                {
                    PlayerInfoItem->SetPlayerState(CB_PS);

                    if (CB_PS->TeamIndex == 1)
                    {
                        Team1Container->AddChild(PlayerInfoItem);
                    }
					else if (CB_PS->TeamIndex == 2)
					{
						Team2Container->AddChild(PlayerInfoItem);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Invalid team index"));
					}

                }
            }
        }
	}
}
