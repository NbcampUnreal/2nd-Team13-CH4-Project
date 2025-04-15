#include "CB_PlayerController.h"

void ACB_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputEnabled(false); // ���� �� ��Ȱ��ȭ
}

void ACB_PlayerController::SetInputEnabled(bool bEnable)
{
    if (APawn* MyPawn = GetPawn())
    {
        MyPawn->EnableInput(bEnable ? this : nullptr);
    }
}