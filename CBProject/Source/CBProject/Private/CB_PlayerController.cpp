#include "CB_PlayerController.h"
#include "EnhancedInputSubsystems.h"

void ACB_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputEnabled(false); //���� �� ��Ȱ��ȭ

    // ���� PlayerController�� ����� Local Player ��ü�� ������    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player���� EnhancedInputLocalPlayerSubsystem�� ȹ��
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
                // �켱����(Priority)�� 0�� ���� ���� �켱����
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

ACB_PlayerController::ACB_PlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    DashAction(nullptr),
    CrouchAction(nullptr)
{
}
