#include "CB_HealthPotion.h"

#include "CB_MovingPlatform.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACB_HealthPotion::ACB_HealthPotion()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	//리플리케이션 설정
	bReplicates = true;
	SetReplicateMovement(false);

	//컴포넌트
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	RootComponent = SphereCollider;

	//스피어 콜라이더
	SphereCollider->SetSphereRadius(75.0f);
	SphereCollider->SetCollisionProfileName(TEXT("Trigger"));
	SphereCollider->SetGenerateOverlapEvents(true);

	PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	PotionMesh->SetupAttachment(RootComponent);
	PotionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealAmount = 30.0f;
}


void ACB_HealthPotion::BeginPlay()
{
	Super::BeginPlay();

	if (SphereCollider)
	{
		SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ACB_HealthPotion::OnOverlapBegin);
	}
}

void ACB_HealthPotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		// 오버랩되 엑터 유효한가?, 자신이 오버랩이 아니며, 폰이냐?
		if (OtherActor && OtherActor != this && OtherActor->IsA<APawn>())
		{
			// 힐 시스템 구현했는가?(힐 인터페이스)
			// 인터페이스를 구현했는지 확인
			// if (IHealableInterface* HealableActor = Cast<IHealableInterface>(OtherActor))
			// {
			// 	// 인터페이스 함수 호출하여 치유
			// 	if (IHealableInterface::Execute_ApplyHealthGain(OtherActor, HealAmount)) // OtherActor가 인터페이스를 구현했으므로 해당 객체에서 실행
			// 	{
			// 		UE_LOG(LogTemp, Log, TEXT("Potion %s applying %f heal to %s"), *GetName(), HealAmount, *OtherActor->GetName());
			//
			// 		// 치유 성공 시 멀티캐스트 함수 호출 (사운드 재생)
			// 		MulticastPlayPickupEffects();
			//
			// 		Destroy();
			// 	}
			// 	else
			// 	{
			// 		// 힐 못했음?
			// 		UE_LOG(LogTemp, Log, TEXT("Potion %s failed to apply heal to %s (Already full health or invalid amount?)"), *GetName(), HealAmount, *OtherActor->GetName());
			// 	}
			// }
			// else
			// {
			// 	//인터페이스 구현 못했음?
			// 	UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor %s does not implement IHealableInterface"), *OtherActor->GetName());
			// }
		}
	}
}
		
void ACB_HealthPotion::MulticastPlayPickupEffects_Implementation()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

void ACB_HealthPotion::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("Potion %s overlapped with %s on %s"), *GetName(),  *OtherActor->GetName(), HasAuthority() ? TEXT("Server") : TEXT("Client"));
}


