#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CB_FigtherCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Punch,
	Kick
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Standing,
	Crouching,
	Jumping
};

UCLASS()
class CBPROJECT_API ACB_FigtherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACB_FigtherCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montages")
	UAnimMontage* StandingPunchMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montages")
	UAnimMontage* CrouchPunchMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montages")
	UAnimMontage* JumpPunchMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montages")
	UAnimMontage* StandingKickMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montages")
	UAnimMontage* CrouchKickMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Montages")
	UAnimMontage* JumpKickMontage;


	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	void ReceiveHeal(float HealAmount);

	UFUNCTION(Server, Reliable)
	void ServerPunch(ECharacterState State);

	UFUNCTION(Server, Reliable)
	void ServerKick(ECharacterState State);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackAnim(EAttackType AttackType, ECharacterState State);

	UFUNCTION()
	void OnRep_Health();

	void TryPerformingDropDown();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsStandingOnOneWayPlatform(UPrimitiveComponent*& OutPlatformComponent);

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDashing = false;

	ECharacterState GetCurrentCharacterState() const;
};
