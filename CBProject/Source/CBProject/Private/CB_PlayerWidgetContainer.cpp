// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerWidgetContainer.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/World.h"
#include "CB_GameState.h"
#include "Kismet/GameplayStatics.h"

void UCB_PlayerWidgetContainer::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePlayerList();

	if (!PlayerInfoContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInfoContainer is NULL"));
	}
	/*
	if (!Team1Container)
	{
		UE_LOG(LogTemp, Error, TEXT("Team1Container is NULL"));
	}
	if (!Team2Container)
	{
		UE_LOG(LogTemp, Error, TEXT("Team2Container is NULL"));
	}
	*/
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
				UE_LOG(LogTemp, Error, TEXT("캐스팅 실패"));
				continue;
			}

			UE_LOG(LogTemp, Warning, TEXT("플레이어 이름: %s, 팀 인덱스: %d"), *CB_PS->PlayerName, CB_PS->TeamIndex);

            if (PlayerInfoItemClass)
            {
                UCB_PlayerInfoWidget* PlayerInfoItem = CreateWidget<UCB_PlayerInfoWidget>(GetWorld(), PlayerInfoItemClass);

				if (PlayerInfoItem)
				{
					UE_LOG(LogTemp, Warning, TEXT("PlayerInfoItem 생성 성공: %s"), *CB_PS->PlayerName);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerInfoItem 생성 실패"));
				}
                if (PlayerInfoItem)
                {
                    PlayerInfoItem->SetPlayerState(CB_PS);
					PlayerInfoContainer->AddChild(PlayerInfoItem);
					/*
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
					*/

                }
            }
        }
	}
}
