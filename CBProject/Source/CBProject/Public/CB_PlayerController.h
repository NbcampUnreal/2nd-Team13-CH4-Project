// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CB_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class CBPROJECT_API ACB_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACB_PlayerController();
    virtual void BeginPlay() override;
    void SetInputEnabled(bool bEnable);
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(Client, Reliable)
    void ClientSetCamera(AActor* CameraActor);

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

    void ResetDash();

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

private:

    FVector DashStartLocation;
    FVector DashTargetLocation;
    float DashInterpAlpha = 0.f;
    float DashSpeed = 2500.f;
    float DashDistance = 400.f;

    bool bCanDash = true;             // Dash 가능 여부
    bool bIsDashing = false;          // 현재 Dash 중인지
    FTimerHandle DashCooldownHandle;  // 쿨타임 타이머


};
