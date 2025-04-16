#include "CB_FigtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ACB_FigtherCharacter::ACB_FigtherCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 앉기 기능 활성화
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void ACB_FigtherCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ACB_FigtherCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACB_FigtherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 좌우 이동
    PlayerInputComponent->BindAxis("MoveRight", this, &ACB_FigtherCharacter::MoveRight);

    // 점프
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACB_FigtherCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACB_FigtherCharacter::StopJumping);

    // 앉기 (S 키)
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACB_FigtherCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACB_FigtherCharacter::StopCrouch);
}

// 좌우이동 고정시켜버리기
void ACB_FigtherCharacter::MoveRight(float Value)
{
    AddMovementInput(FVector(1, 0, 0), Value);
}

void ACB_FigtherCharacter::StartCrouch()
{
    Crouch();
}

void ACB_FigtherCharacter::StopCrouch()
{
    UnCrouch();
}

