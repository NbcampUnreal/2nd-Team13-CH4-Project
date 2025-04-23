#include "CB_FigtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterMovementComponentAsync.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "CB_PlayerState.h"
#include "Engine/DamageEvents.h"
#include "CB_GameMode.h"

ACB_FigtherCharacter::ACB_FigtherCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	bReplicates = true;
	MaxHealth = 100.f;
	CurrentHealth = 30.f;
}

void ACB_FigtherCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACB_FigtherCharacter, CurrentHealth);

}

void ACB_FigtherCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Tags.Add("Player");
}

float ACB_FigtherCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("HP: %f"), CurrentHealth));

	if (CurrentHealth <= 0.0f)
	{
		ACB_GameMode* GameMode = GetWorld()->GetAuthGameMode<ACB_GameMode>();
		if (GameMode)
		{
			// GameMode->PlayerKilled(EventInstigator, GetController());
		}
	}

	return DamageAmount;
}

void ACB_FigtherCharacter::ReceiveHeal(float HealAmount)
{
	if (!HasAuthority()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("HP: %f"), CurrentHealth));
}

void ACB_FigtherCharacter::Attack_Implementation()
{
	MulticastPlayAttackAnim();

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 200.0f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, QueryParams))
	{
		ACB_FigtherCharacter* Target = Cast<ACB_FigtherCharacter>(HitResult.GetActor());
		if (Target)
		{
			ACB_PlayerState* MyPS = Cast<ACB_PlayerState>(GetPlayerState());
			ACB_PlayerState* TargetPS = Cast<ACB_PlayerState>(Target->GetPlayerState());

			if (MyPS && TargetPS && MyPS->TeamIndex != TargetPS->TeamIndex)
			{
				FDamageEvent DamageEvent;
				Target->TakeDamage(100.0f, DamageEvent, GetController(), this);
			}
		}
	}
}


void ACB_FigtherCharacter::MulticastPlayAttackAnim_Implementation()
{
	//if (AttackMontage)
	//{
	//	PlayAnimMontage(AttackMontage);
	//}
}


void ACB_FigtherCharacter::OnRep_Health()
{
	// Client 내에  UI 반영
}

void ACB_FigtherCharacter::TryPerformingDropDown()
{
	UPrimitiveComponent* PlatformComponent = nullptr;
	if (IsStandingOnOneWayPlatform(PlatformComponent) && PlatformComponent != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Character performing drop down from platform: %s"), *PlatformComponent->GetName());
		if (UCharacterMovementComponent* CMC = GetCharacterMovement())
		{
			CMC->SetMovementMode(MOVE_Falling);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Character cannot DropDown: Not standing on a valid one-way platform."));
	}
}

// 땅 위에 서 있는지 확인하는 함수
bool ACB_FigtherCharacter::IsStandingOnOneWayPlatform(UPrimitiveComponent*& OutPlatformComponent)
{
	// 초기화
	OutPlatformComponent = nullptr;

	// 캐릭터 MovementComponent가져오기
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	// 없으면 돌아가
	if (!CMC)
	{
		return false;
	}

	// 캐릭터의 캡슐 컴포넌트 가져오기 (FindFloor에 필요)
	const UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (!CapsuleComp)
	{
		return false;
	}
	// 바닥 찾기 결과 정보 저장용
	FFindFloorResult FloorResult;
	
	float SearchDistance = 10.0f; // 발 아래 얼마나 멀리까지 바닥을 찾을지 거리
	bool bCanUseCachedResult = false; // 캐시된 결과 사용 안 함 (새로 찾기)

	// FindFloor 함수 호출: 현재 캡슐 위치를 기준으로 아래 방향으로 바닥을 찾음
	CMC->FindFloor(CapsuleComp->GetComponentLocation(), FloorResult, bCanUseCachedResult, nullptr);
	
	// FindFloor가 걸을 수 있는 유효한 바닥을 찾았는지 확인
	if (FloorResult.IsWalkableFloor())
	{
		
		// 바닥 정보를 구성하는 콜리전 컴포넌트 가져오기 (FHitResult 사용)
		UPrimitiveComponent* FloorComponent = FloorResult.HitResult.GetComponent();
		if (FloorComponent)
		{
			// 발판 컴포넌트의 콜리전 오브젝트 타입 가져오기
			ECollisionChannel FloorObjectChannel = FloorComponent->GetCollisionObjectType();
			
			// 가져온 오브젝트 타입이 원웨이 플랫폼 채널(예: ECC_GameTraceChannel1)과 같은지 비교
			if (FloorObjectChannel == ECC_GameTraceChannel1) // ECC_GameTraceChannel1은 DefaultEngine.ini 매핑 채널
			{
				OutPlatformComponent = FloorComponent; // 찾은 컴포넌트 저장
				return true; // 원웨이 플랫폼 위에 있음
			}
			
		}
	}
	// 모든 조건 통과 못하면 서 있지 않은 것으로 false반환
	return false;
}