#include "CB_FigtherCharacter.h"

ACB_FigtherCharacter::ACB_FigtherCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACB_FigtherCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ACB_FigtherCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}