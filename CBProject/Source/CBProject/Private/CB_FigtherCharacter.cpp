#include "CB_FigtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ACB_FigtherCharacter::ACB_FigtherCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

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