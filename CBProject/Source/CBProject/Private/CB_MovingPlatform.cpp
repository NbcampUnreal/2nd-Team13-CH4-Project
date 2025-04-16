#include "CB_MovingPlatform.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ACB_MovingPlatform::ACB_MovingPlatform()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

	// 루트 컴포넌트 생성 및 설정
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot; 

	// 스태틱 메시 컴포넌트 생성
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);

	bReplicates = true;
	bIsActive = bStartActive;
}



void ACB_MovingPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// bIsActive 변수를 모든 클라이언트에게 리플리케이트하도록 등록
	DOREPLIFETIME(ACB_MovingPlatform, bIsActive);
}

void ACB_MovingPlatform::OnRep_IsActive()
{
	// 클라이언트에서 bIsActive 값이 서버로부터 복제되어 변경되었을 때 호출됨
	// 예를 들어, 활성화/비활성화에 따른 이펙트나 사운드를 여기서 재생할 수 있음
	if (bIsActive)
	{
		UE_LOG(LogTemp, Log, TEXT("%s - Client Received Activation"), *GetName());
		// Play Activation Effect/Sound
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s - Client Received Deactivation"), *GetName());
		// Play Deactivation Effect/Sound
	}
}

void ACB_MovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) // HasAuthority()는 서버인지 확인
	{
		// 시작 위치와 목표 위치 설정 (월드 좌표 기준)
		GlobalStartLocation = GetActorLocation();
		// TargetRelativeLocation을 월드 좌표로 변환 (액터의 트랜스폼 기준)
		GlobalTargetLocation = GetTransform().TransformPosition(TargetRelativeLocation);

		// 초기 이동 방향 계산
		MoveDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();

		bIsActive = bStartActive; // 시작 시 활성화 여부 설정
		CurrentWaitTime = 0.0f;
		bMovingToTarget = true;
	}
}


void ACB_MovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		if (bIsActive) // 활성화 상태일 때만 이동 로직 실행
		{
			// 대기 중인지 확인
			if (CurrentWaitTime > 0.0f)
			{
				CurrentWaitTime -= DeltaTime;
				return; // 대기 시간이 끝나기 전까지는 더 이상 진행하지 않음
			}

			// 현재 위치와 목표 위치 설정
			FVector CurrentLocation = GetActorLocation();
			FVector TargetLocation = bMovingToTarget ? GlobalTargetLocation : GlobalStartLocation;

			// 목표 지점까지 남은 거리
			float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);

			// 이번 프레임에 이동할 거리
			float DistanceToMove = MoveSpeed * DeltaTime;

			// 목표 지점에 거의 도달했거나 넘어섰는지 확인
			if (DistanceToMove >= DistanceToTarget)
			{
				// 목표 지점에 정확히 위치시킴
				SetActorLocation(TargetLocation);

				// PingPong 모드 활성화 시
				if (bPingPong)
				{
					// 대기 시간 시작
					CurrentWaitTime = WaitTime;
					// 이동 방향 전환
					bMovingToTarget = !bMovingToTarget;
					// 새 방향 벡터 계산 (반대 방향)
					MoveDirection = (bMovingToTarget ? GlobalTargetLocation - GlobalStartLocation : GlobalStartLocation - GlobalTargetLocation).GetSafeNormal();
				}
				else // PingPong 모드가 아니면 도착 후 정지
				{
					bIsActive = false; // 비활성화
				}
			}
			else // 아직 목표 지점에 도달하지 못했으면 계속 이동
			{
				FVector NewLocation = CurrentLocation + MoveDirection * DistanceToMove;
				SetActorLocation(NewLocation);
			}
		}
	}
}

void ACB_MovingPlatform::ActivatePlatform_Implementation()
{
	if (HasAuthority())
	{
		bIsActive = true;
		CurrentWaitTime = 0.0f; // 즉시 이동 시작하도록 대기 시간 초기화
		UE_LOG(LogTemp, Log, TEXT("%s Activated"), *GetName());
	}
	
}

void ACB_MovingPlatform::DeactivatePlatform_Implementation()
{
	if (HasAuthority())
	{
		bIsActive = false;
		UE_LOG(LogTemp, Log, TEXT("%s Deactivated"), *GetName());
	}
}

bool ACB_MovingPlatform::IsPlatformActive_Implementation()
{
	return bIsActive;
}