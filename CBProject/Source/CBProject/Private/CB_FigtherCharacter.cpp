#include "CB_FigtherCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ACB_FigtherCharacter::ACB_FigtherCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // �ɱ� ��� Ȱ��ȭ
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

    // �¿� �̵�
    PlayerInputComponent->BindAxis("MoveRight", this, &ACB_FigtherCharacter::MoveRight);

    // ����
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACB_FigtherCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACB_FigtherCharacter::StopJumping);

    // �ɱ� (S Ű)
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACB_FigtherCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACB_FigtherCharacter::StopCrouch);
}

// �¿��̵� �������ѹ�����
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

