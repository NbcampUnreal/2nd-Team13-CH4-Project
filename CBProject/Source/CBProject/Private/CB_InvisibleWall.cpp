#include "CB_InvisibleWall.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"  

ACB_InvisibleWall::ACB_InvisibleWall()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetCollisionProfileName(TEXT("Custom"));

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCanEverAffectNavigation(false);

	CollisionBox->SetCollisionObjectType(ECC_WorldStatic);
	
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetHiddenInGame(true);
	CollisionBox->SetMobility(EComponentMobility::Static);
	CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f));
}



