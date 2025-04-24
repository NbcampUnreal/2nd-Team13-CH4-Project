#include "CB_NormalPlatform.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"

ACB_NormalPlatform::ACB_NormalPlatform()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	PlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OneWayCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OneWayCollisionBox"));
	OneWayCollisionBox->SetupAttachment(RootComponent);

	OneWayCollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	OneWayCollisionBox->SetBoxExtent(FVector(50.0f, 100.0f, 5.0f));

	OneWayCollisionBox->SetCollisionProfileName(TEXT("Custom"));
	OneWayCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OneWayCollisionBox->SetCanEverAffectNavigation(false);

	OneWayCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OneWayCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OneWayCollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	bReplicates = true;

}

void ACB_NormalPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

