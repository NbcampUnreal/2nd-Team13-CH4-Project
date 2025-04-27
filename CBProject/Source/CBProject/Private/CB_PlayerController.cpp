#include "CB_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CB_FigtherCharacter.h"
#include "CB_GameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CB_PlayerWidgetContainer.h"

ACB_PlayerController::ACB_PlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    DashAction(nullptr),
    CrouchAction(nullptr),
    PunchAction(nullptr),
    KickAction(nullptr),
    bCanDash(true),
    bIsDashing(false)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    SetActorTickEnabled(true);
}

void ACB_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    UClass* UIManagerClass = StaticLoadClass(
        UCB_UIManager::StaticClass(),
        nullptr,
        TEXT("/Game/Blueprints/BP_CB_UIManager.BP_CB_UIManager_C")
    );

    if (UIManagerClass)
    {
        if (IsLocalPlayerController())
        {
            UIManager = NewObject<UCB_UIManager>(this, UIManagerClass);
            UIManager->Initialize(this);
            UIManager->ShowMainMenu();
        }
    }

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
        }, 2.0f, false);
    }
}

UCB_UIManager* ACB_PlayerController::GetUIManager() const
{
    return UIManager;
}

void ACB_PlayerController::ServerStartGame_Implementation()
{
    MulticastStartGame();
}

void ACB_PlayerController::MulticastStartGame_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("모든 클라이언트에 게임 시작"));
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
      
        }
        if (DropDownAction)
        {
            EnhancedInput->BindAction(DropDownAction, ETriggerEvent::Triggered, this, &ACB_PlayerController::HandleDropDownInput);
        }
        if (PunchAction)
        {
            EnhancedInput->BindAction(PunchAction, ETriggerEvent::Started, this, &ACB_PlayerController::AttackPunch);
        }
        if (KickAction)
        {
            EnhancedInput->BindAction(KickAction, ETriggerEvent::Started, this, &ACB_PlayerController::AttackKick);
        }
    }
}

void ACB_PlayerController::Move(const FInputActionValue& Value)
{
    if (bIsDashing) return;

    FVector2D MovementVector = Value.Get<FVector2D>();
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    // 대난투 스타일: 월드 기준 X축 방향(좌우) 이동만 허용
    FVector Direction = FVector::ForwardVector; // (1, 0, 0)
    ControlledPawn->AddMovementInput(Direction, MovementVector.X);

    // 바라보는 방향 설정
    if (MovementVector.X != 0.f)
    {
        FRotator NewRotation = Direction.Rotation();
        if (MovementVector.X < 0.f)
            NewRotation.Yaw += 180.f;

        ControlledPawn->SetActorRotation(NewRotation);
    }
}


void ACB_PlayerController::StartJump(const FInputActionValue& Value)
{
    if (bIsDashing) return;

    FVector2D MovementVector = Value.Get<FVector2D>();
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
        ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
    }

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
    if (bIsDashing) return;

    FVector2D MovementVector = Value.Get<FVector2D>();
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
        ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
    }

    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        if (!Fighter->GetCharacterMovement()->IsFalling())
        {
            Fighter->Crouch();
        }
    }
}

void ACB_PlayerController::StopCrouch(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->UnCrouch();
    }
}

void ACB_PlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsDashing)
    {
        APawn* ControlledPawn = GetPawn();
        if (!ControlledPawn) return;

        DashInterpAlpha += DeltaTime * (DashSpeed / DashDistance);
        FVector NewLocation = FMath::Lerp(DashStartLocation, DashTargetLocation, DashInterpAlpha);
        ControlledPawn->SetActorLocation(NewLocation, true);

        if (DashInterpAlpha >= 1.f)
        {
            bIsDashing = false;

            if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(ControlledPawn))
            {
                Fighter->bIsDashing = false;
            }
        }
    }
}


void ACB_PlayerController::ResetDash()
{
    bCanDash = true;
}

void ACB_PlayerController::StartDash(const FInputActionValue& Value)
{
    if (!bCanDash || bIsDashing)
    {
        return;
    }

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    bCanDash = false;
    bIsDashing = true;
    DashInterpAlpha = 0.f;

    DashStartLocation = ControlledPawn->GetActorLocation();
    DashTargetLocation = DashStartLocation + ControlledPawn->GetActorForwardVector() * DashDistance;

    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(ControlledPawn))
    {
        Fighter->bIsDashing = true;
    }

    GetWorld()->GetTimerManager().SetTimer(DashCooldownHandle, this, &ACB_PlayerController::ResetDash, 1.0f, false);
}

void ACB_PlayerController::HandleDropDownInput(const FInputActionValue& Value)
{
    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        Fighter->TryPerformingDropDown();
    }
}


void ACB_PlayerController::ClientCreatePlayerInfoUI_Implementation()
{
    if (IsLocalController())
    {
        if (PlayerWidgetContainerClass)
        {
            PlayerWidgetContainer = CreateWidget<UCB_PlayerWidgetContainer>(this, PlayerWidgetContainerClass);
            if (PlayerWidgetContainer)
            {
                PlayerWidgetContainer->AddToViewport();
                PlayerWidgetContainer->UpdatePlayerList();
            }
            else
            {
                UE_LOG(LogTemp, Display, TEXT("PlayerWidgetContainer Create fail"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("PlayerWidgetContainerClass Not invalid"));

        }
    }
}

void ACB_PlayerController::AttackPunch(const FInputActionValue& Value)
{
    
    if (bIsDashing)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputMappingContext is NULL!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("J key pressed!"));

    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        ECharacterState State = Fighter->GetCurrentCharacterState();

        UE_LOG(LogTemp, Warning, TEXT("[Attack] Punch input received, State: %d"), (uint8)State);

        Fighter->ServerPunch(State);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot attack while dashing"));
    }
}


void ACB_PlayerController::AttackKick(const FInputActionValue& Value)
{
    if (bIsDashing) return;

    UE_LOG(LogTemp, Warning, TEXT("K key pressed!"));

    if (ACB_FigtherCharacter* Fighter = Cast<ACB_FigtherCharacter>(GetPawn()))
    {
        ECharacterState State = Fighter->GetCurrentCharacterState();
        UE_LOG(LogTemp, Warning, TEXT("[Attack] Kick input received, State: %d"), (uint8)State);

        Fighter->ServerKick(State);
    }
}
