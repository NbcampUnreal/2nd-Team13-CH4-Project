#include "SM_Controller.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CB_FigtherCharacter.h"
#include "CB_GameState.h"

ASM_Controller::ASM_Controller()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    DashAction(nullptr),
    CrouchAction(nullptr),
    //이 밑에 추가함
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr)
    // 이 위에 추가함
{
}

// 여기 밑에 추가함
void ASM_Controller::ShowMainMenu()
{
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
}

// 여기 위에 추가함 / 위에 생성자에도 몇개 추가함

void ASM_Controller::BeginPlay()
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

    if (IsLocalController())
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
            {
                ACB_GameState* GS = GetWorld()->GetGameState<ACB_GameState>();
                UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay : %s"),
                    *GetNameSafe(GS->SharedCameraActor));
                if (GS && GS->SharedCameraActor)
                {
                    SetViewTargetWithBlend(GS->SharedCameraActor, 0.5f);
                    UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay 1 : %s"),
                        *GetNameSafe(this));
                }
            }, 1.0f, false);
    }
}

void ASM_Controller::SetInputEnabled(bool bEnable)
{
    if (APawn* MyPawn = GetPawn())
    {
        MyPawn->EnableInput(bEnable ? this : nullptr);
    }
}

void ASM_Controller::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASM_Controller::Move);
        }
        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASM_Controller::StartJump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASM_Controller::StopJump);
        }
        if (CrouchAction)
        {
            EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASM_Controller::StartCrouch);
            EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASM_Controller::StopCrouch);
        }
        if (DashAction)
        {
            EnhancedInput->BindAction(DashAction, ETriggerEvent::Started, this, &ASM_Controller::StartDash);
            EnhancedInput->BindAction(DashAction, ETriggerEvent::Completed, this, &ASM_Controller::StopDash);
        }
    }
}

void ASM_Controller::Move(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        float AxisValue = Value.Get<float>();
        ControlledPawn->AddMovementInput(FVector(1.f, 0.f, 0.f), AxisValue);
    }
}

void ASM_Controller::StartJump(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->Jump();
    }
}

void ASM_Controller::StopJump(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->StopJumping();
    }
}

void ASM_Controller::StartCrouch(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->Crouch();

    }
}

void ASM_Controller::StopCrouch(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->UnCrouch();
    }
}

void ASM_Controller::StartDash(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Dash Start"));

}

void ASM_Controller::StopDash(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Dash end"));
}