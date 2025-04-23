#include "CB_GameMode.h"
#include "CB_GameState.h"
#include "CB_PlayerController.h"
#include "CB_PlayerState.h"
#include "DynamicCameraActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "CB_PlayerController.h"
#include "CB_GameState.h"

ACB_GameMode::ACB_GameMode()
{

}

void ACB_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//맵에 배치된 SpawnPoint 태그를 가진 액터 모으기
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);

	if (HasAuthority() && CameraActorClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ADynamicCameraActor* Cam = GetWorld()->SpawnActor<ADynamicCameraActor>(
			CameraActorClass, 
			FVector(0, 0, 300), 
			FRotator(0, 0, 0), 
			Params
		);
	
		if (Cam)
		{
			Cam->SetReplicates(true);
			Cam->SetReplicateMovement(false);

			if (ACB_GameState* GS = GetGameState<ACB_GameState>())
			{
				GS->SharedCameraActor = Cam;
				GS->ForceNetUpdate();
			}
		}
	}
}

void ACB_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		ACB_GameState* GS = GetGameState<ACB_GameState>();
		ACB_PlayerController* PC = Cast<ACB_PlayerController>(NewPlayer);
		UE_LOG(LogTemp, Warning,
			TEXT("PostLogin for %s (Cast %s) → camera=%s"),
			*GetNameSafe(NewPlayer),
			PC ? TEXT("OK") : TEXT("FAIL"),
			*GetNameSafe(GS ? GS->SharedCameraActor : nullptr));

		UE_LOG(LogTemp, Warning, TEXT("PostLogin RPC to client : %s"),
			*GetNameSafe(GS->SharedCameraActor));

		if (!GS)
		{
			return;
		}

		if (GS && PC && GS->SharedCameraActor)
		{
			//PC->ClientSetCamera(GS->SharedCameraActor);
		}

	}
}

void ACB_GameMode::StartPlay()
{
	Super::StartPlay();

	CountdownTime = 3;

	UpdateCountdownUI();

	// 카운트 다운 시작(3초)
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, 
		this, 
		&ACB_GameMode::TickCountdown, 
		1.0f, 
		true);

	//GameState에 카운트 다운 시작 알림
	if (ACB_GameState* CB_GameState = GetGameState<ACB_GameState>())
	{
		CB_GameState->bIsCountdownRunning = true;
	}
}

void ACB_GameMode::TickCountdown()
{
	CountdownTime--;

	UpdateCountdownUI();

	if (CountdownTime < 0)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		StartGame();
	}
}

void ACB_GameMode::UpdateCountdownUI()
{
	//for (APlayerController* PC : TActorRange<APlayerController>(GetWorld()))
	//{
	//	if (ACB_PlayerController* CB_PC = Cast<ACB_PlayerController>(PC))
	//	{
	//		if (UCB_CountdownWidget* CountdownWidget = CB_PC->GetCountdownWidget())
	//		{
	//			CountdownWidget->UpdateCountdownImage(CountdownTime);
	//		}
	//	}
	//}
}

void ACB_GameMode::StartGame()
{
	if (ACB_GameState* CB_GameState = GetGameState<ACB_GameState>())
	{
		CB_GameState->bIsCountdownRunning = false;
	}

	//모든 플레이어 입력 활성화
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(*It);
		if (PlayerController)
		{
			PlayerController->SetInputEnabled(true);
		}
	}
}

void ACB_GameMode::RestartPlayer(AController* NewPlayer)
{	//재시작 하면 SpawnPoint의 태그를 가진 랜덤 스폰위치로 소환
	if (SpawnPoints.Num() > 0)
	{
		AActor* ChosenSpawn = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
		RestartPlayerAtPlayerStart(NewPlayer, ChosenSpawn);
	}
	else
	{
		Super::RestartPlayer(NewPlayer);
	}
}

void ACB_GameMode::HandleRespawn(AController* Controller)
{	//3초 후 리스폰
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle, 
		[this, Controller]() {RestartPlayer(Controller);},
		3.0f,
		false);
}

void ACB_GameMode::HandlePlayerDeath(AController* DeadController)
{
	ACB_PlayerState* CB_PlayerState = DeadController->GetPlayerState<ACB_PlayerState>();
	if (CB_PlayerState && --CB_PlayerState->Lives > 0)
	{
		//리스폰 처리
		RespawnPlayer(DeadController);
	}
	else
	{
		//관전모드
		DeadController->ChangeState(NAME_Spectating);
	}
}

void ACB_GameMode::OnRep_CountdownChanged()
{
	// 클라에서 UI 반영 용도 (HUD에서 타이머 시작)
}

void ACB_GameMode::SetTeam(AController* NewPlayer)
{
	int32 TeamForPlayer = GetNumPlayers() % 2;

	if (ACB_PlayerState* PS = NewPlayer->GetPlayerState<ACB_PlayerState>())
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 %s를 팀 %d에 배정"), *NewPlayer->GetName(), TeamForPlayer);
		PS->TeamIndex = TeamForPlayer;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
			{
				ACB_PlayerController* PC = Cast<ACB_PlayerController>(NewPlayer);
				if (PC)
				{
					UE_LOG(LogTemp, Warning, TEXT("Client Player Info UI"));
					PC->ClientCreatePlayerInfoUI();

					for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
					{
						ACB_PlayerController* OtherPC = Cast<ACB_PlayerController>(*It);
						if (OtherPC && OtherPC != PC)
						{
							OtherPC->ClientCreatePlayerInfoUI();
						}
					}
				}
			}, 1.0f, false);
	}
}

void ACB_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACB_GameMode, bIsCountdownRunning);
}

void ACB_GameMode::RespawnPlayer(AController* Controller)
{
	//Life 차감 및 스폰 포인트에서 리스폰
}