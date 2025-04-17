#include "CB_FigtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CB_PlayerState.h"
#include "Engine/DamageEvents.h"
#include "CB_GameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

ACB_FigtherCharacter::ACB_FigtherCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	bReplicates = true;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
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
	// Client ?êÏÑú Health Í∞?Î∞îÎÄ???UI,?¥Ìå©?? ?ºÍ≤© Î∞òÏùë Ï∂úÎ†•
}

