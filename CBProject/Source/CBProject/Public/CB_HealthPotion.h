#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CB_HealthPotion.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class CBPROJECT_API ACB_HealthPotion : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACB_HealthPotion();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 상호작용 범위 감지할 스피어 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereCollider;
	// 물약 껍데기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PotionMesh;
	// 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	float HealAmount = 30.0f;
	//사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class USoundBase* PickupSound;

	// 다른 클라에게 재생하기 위한 멀티캐스트
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayPickupEffects();
	
public:	
	
	virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;

};
