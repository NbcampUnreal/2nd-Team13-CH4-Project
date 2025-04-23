#include "CB_FigtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterMovementComponentAsync.h"
#include "Engine/EngineTypes.h"
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
		UE_LOG(LogTemp, Error, TEXT("IsStandingOnOneWayPlatform: CharacterMovementComponent is NULL!"));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("IsStandingOnOneWayPlatform: CMC is valid. Trying to access CurrentFloor..."));
	}
	
	// 걷고 있는지 상태 확인
	if (CMC->IsMovingOnGround())
	{
		// 현재 캐릭터의 발 아래 있는 바닥 정보 가져오기 
		const FFindFloorResult& CurrentFloor = CMC->CurrentFloor;
		UE_LOG(LogTemp, Log, TEXT("IsStandingOnOneWayPlatform: Accessed CurrentFloor successfully."));
		// 걸을수 있는 유요한 표면인가?
		if (CurrentFloor.IsWalkableFloor())
		{
			// 바닥 콜리전 컴포넌트 가져오기
			UPrimitiveComponent* FloorComponent = CurrentFloor.HitResult.GetComponent();
			// 컴포넌트 유효해?
			if (FloorComponent)
			{
				// 발판 컴포넌트의 콜리전 오브젝트 타입 가져옴
				ECollisionChannel FloorObjectChannel = FloorComponent->GetCollisionObjectType();
				// 가져온 옵젝 타입이 플랫폼으로 지정한 채널과 같은지 비교
				if (FloorObjectChannel == ECC_GameTraceChannel1)
				{
					// 일치하면 찾은 컴포넌트를 출력 컴포넌트에 저장
					OutPlatformComponent = FloorComponent;

					return true;
				}
			}
		}
	}
	// 모든 조건 통과 못하면 서 있지 않은 것으로 false반환
	return false;
}