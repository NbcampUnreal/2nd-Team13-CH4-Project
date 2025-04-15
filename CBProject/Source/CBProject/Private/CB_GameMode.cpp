#include "CB_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CB_GameState.h"
#include "CB_PlayerController.h"
#include "Net/UnrealNetwork.h"

void ACB_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//�ʿ� ��ġ�� SpawnPoint �±׸� ���� ���� ������
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);
}

void ACB_GameMode::StartPlay()
{
	Super::StartPlay();

	// ī��Ʈ �ٿ� ����(3��)
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, 
		this, 
		&ACB_GameMode::StartGame, 
		3.0f, 
		false);

	//GameState�� ī��Ʈ �ٿ� ���� �˸�
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

	//��� �÷��̾� �Է� Ȱ��ȭ
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
{	//����� �ϸ� SpawnPoint�� �±׸� ���� ���� ������ġ�� ��ȯ
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
{	//3�� �� ������
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle, 
		[this, Controller]() {RestartPlayer(Controller);},
		3.0f,
		false);
}

void ACB_GameMode::OnRep_CountdownChanged()
{
	// Ŭ�󿡼� UI �ݿ� �뵵 (HUD���� Ÿ�̸� ����)
}

void ACB_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACB_GameMode, bIsCountdownRunning);
}
