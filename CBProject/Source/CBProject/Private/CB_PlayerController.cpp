#include "CB_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CB_FigtherCharacter.h"

ACB_PlayerController::ACB_PlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    DashAction(nullptr),
    CrouchAction(nullptr)
{
}

void ACB_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputEnabled(true); // 테스트 중에는 true

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}

void ACB_PlayerController::SetInputEnabled(bool bEnable)
{
    if (APawn* MyPawn = GetPawn())
    {
        MyPawn->EnableInput(bEnable ? this : nullptr);
    }
}

void ACB_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACB_PlayerController::Move);
        }
        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACB_PlayerController::StartJump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACB_PlayerController::StopJump);
        }
        if (CrouchAction)
        {
            EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACB_PlayerController::StartCrouch);
            EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACB_PlayerController::StopCrouch);
        }
        if (DashAction)
        {
            EnhancedInput->BindAction(DashAction, ETriggerEvent::Started, this, &ACB_PlayerController::StartDash);
            EnhancedInput->BindAction(DashAction, ETriggerEvent::Completed, this, &ACB_PlayerController::StopDash);
        }
    }
}

void ACB_PlayerController::Move(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        float AxisValue = Value.Get<float>();
        ControlledPawn->AddMovementInput(FVector(1.f, 0.f, 0.f), AxisValue);
    }
}

void ACB_PlayerController::StartJump(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->Jump();
    }
}

void ACB_PlayerController::StopJump(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->StopJumping();
    }
}

void ACB_PlayerController::StartCrouch(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->Crouch();
        
    }
}

void ACB_PlayerController::StopCrouch(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->UnCrouch();
    }
}

void ACB_PlayerController::StartDash(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Dash Start"));

}

void ACB_PlayerController::StopDash(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Dash end"));
}