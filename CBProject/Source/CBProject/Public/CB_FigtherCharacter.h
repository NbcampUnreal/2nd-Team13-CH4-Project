#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CB_FigtherCharacter.generated.h"

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

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	void ReceiveHeal(float HealAmount);

	UFUNCTION(Server, Reliable)
	void Attack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackAnim();

	UFUNCTION()
	void OnRep_Health();
};
