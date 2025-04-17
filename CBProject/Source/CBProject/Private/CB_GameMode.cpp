#include "CB_GameMode.h"
#include "CB_GameState.h"
#include "CB_PlayerController.h"
#include "CB_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void ACB_GameMode::BeginPlay()
{
	Super::BeginPlay();

	//�ʿ� ��ġ�� SpawnPoint �±׸� ���� ���� ������
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* Camera = GetWorld()->SpawnActor<AActor>(CameraActorClass, FVector(0, 0, 300), FRotator(-45, 0, 0), SpawnParams);
	
		ACB_GameState* GS = GetGameState<ACB_GameState>();
		if (GS)
		{
			GS->SharedCameraActor = Camera;
		}
	}
}

void ACB_GameMode::StartPlay()
{
	Super::StartPlay();

	CountdownTime = 3;

	UpdateCountdownUI();

	// ī��Ʈ �ٿ� ����(3��)
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle, 
		this, 
		&ACB_GameMode::TickCountdown, 
		1.0f, 
		true);

	//GameState�� ī��Ʈ �ٿ� ���� �˸�
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

void ACB_GameMode::HandlePlayerDeath(AController* DeadController)
{
	ACB_PlayerState* CB_PlayerState = DeadController->GetPlayerState<ACB_PlayerState>();
	if (CB_PlayerState && --CB_PlayerState->Lives > 0)
	{
		//������ ó��
		RespawnPlayer(DeadController);
	}
	else
	{
		//�������
		DeadController->ChangeState(NAME_Spectating);
	}
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

void ACB_GameMode::RespawnPlayer(AController* Controller)
{
	//Life ���� �� ���� ����Ʈ���� ������
}