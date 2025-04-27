#include "CB_GameMode.h"
#include "CB_GameState.h"
#include "CB_PlayerController.h"
#include "CB_PlayerState.h"
#include "DynamicCameraActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "CB_PlayerController.h"
#include "CB_GameState.h"
#include "CB_GameInstance.h"

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
	UE_LOG(LogTemp, Display, TEXT("PostLogin"));
	ACB_PlayerState* CBPS = Cast<ACB_PlayerState>(NewPlayer->PlayerState);
	if (CBPS)
	{
		static int32 NameCounter = 1; // 플레이어별 번호 붙이기

		CBPS->PlayerName = FString::Printf(TEXT("Player %d"), NameCounter++);

		// 디버깅용
		UE_LOG(LogTemp, Display, TEXT("PostLogin Set PlayerNameString: %s"), *CBPS->PlayerName);
	}
}


void ACB_GameMode::StartGame()
{
	CountdownTime = 4;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(*It))
		{
			if (UCB_UIManager* UI = PlayerController->GetUIManager())
			{
				UI->ShowCountdownWidget();     // 위젯 생성 및 표시
				UI->UpdateCountdown(CountdownTime); // 4 → 3 이미지로 시작
			}
		}
	}

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

		//모든 플레이어 입력 활성화
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(*It))
			{
				if (UCB_UIManager* UI = PlayerController->GetUIManager())
				{
					UI->RemoveCountdownWidget(); // 카운트다운 UI 제거
					UI->StartGame();             // 전투 UI로 전환
				}
				PlayerController->SetInputEnabled(true); // 플레이어 입력 활성화
			}
		}
	}

	if (ACB_GameState* CB_GameState = GetGameState<ACB_GameState>())
	{
		CB_GameState->bIsCountdownRunning = false;
	}
}

void ACB_GameMode::UpdateCountdownUI()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ACB_PlayerController* PlayerController = Cast<ACB_PlayerController>(*It))
		{
			if (UCB_UIManager* UI = PlayerController->GetUIManager())
			{
				UI->UpdateCountdown(CountdownTime); // 이미지 변경
			}
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

void ACB_GameMode::SetTeamByMode()
{	
	UCB_GameInstance* GI = Cast<UCB_GameInstance>(GetGameInstance());

	if (!GI)
	{
		return;
	}

	if (GI->GameSelect == 0 || GI->GameSelect)
	{
		for (APlayerState* State : GameState->PlayerArray)
		{
			ACB_PlayerState* PS = Cast<ACB_PlayerState>(State);
			if (PS)
			{
				PS->SetPlayerTeam(0);
			}
		}
	}
	else if (GI->GameSelect == 1)
	{
		for (APlayerState* State : GameState->PlayerArray)
		{
			ACB_PlayerState* PS = Cast<ACB_PlayerState>(State);
			if (PS)
			{
				int32 PlayerNum = GetNumPlayers();
				if (GI->bIsTeamMatch)
				{
					PS->SetPlayerTeam(PlayerNum % 2);
				}
				else
				{
					PS->SetPlayerTeam(PlayerNum);
				}
			}
		}
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