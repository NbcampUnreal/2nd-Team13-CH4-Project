#include "CB_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CB_GameState.h"
#include "CB_PlayerController.h"
#include "Net/UnrealNetwork.h"

void ACB_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//맵에 배치된 SpawnPoint 태그를 가진 액터 모으기
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);
}

void ACB_GameMode::StartPlay()
{
	Super::StartPlay();

	// 카운트 다운 시작(3초)
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, 
		this, 
		&ACB_GameMode::StartGame, 
		3.0f, 
		false);

	//GameState에 카운트 다운 시작 알림
	if (ACB_GameState* CB_GameState = GetGameState<ACB_GameState>())
	{
		CB_GameState->bIsCountdownRunning = true;
	}
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

void ACB_GameMode::OnRep_CountdownChanged()
{
	// 클라에서 UI 반영 용도 (HUD에서 타이머 시작)
}

void ACB_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACB_GameMode, bIsCountdownRunning);
}
