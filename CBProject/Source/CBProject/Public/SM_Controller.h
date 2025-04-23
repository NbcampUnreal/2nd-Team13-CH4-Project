// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CB_UIManager.h"
#include "SM_Controller.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class CBPROJECT_API ASM_Controller : public APlayerController
{
    GENERATED_BODY()

public:
    ASM_Controller();
    virtual void BeginPlay() override;
    void SetInputEnabled(bool bEnable);

    // 여기 밑에만 추가함
    UPROPERTY()
    UCB_UIManager* UIManager;

    UFUNCTION(BlueprintCallable, Category = "UI")
    UCB_UIManager* GetUIManager() const;
    //여기 위에만 추가함

protected:
    virtual void SetupInputComponent() override; // APlayerController는 이걸 오버라이드해야 함

    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UFUNCTION()
    void StartJump(const FInputActionValue& Value);

    UFUNCTION()
    void StopJump(const FInputActionValue& Value);

    UFUNCTION()
    void StartCrouch(const FInputActionValue& Value);

    UFUNCTION()
    void StopCrouch(const FInputActionValue& Value);

    UFUNCTION()
    void StartDash(const FInputActionValue& Value);

    UFUNCTION()
    void StopDash(const FInputActionValue& Value);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* DashAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* CrouchAction;
};
