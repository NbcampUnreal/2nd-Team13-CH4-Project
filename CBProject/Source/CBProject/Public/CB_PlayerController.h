// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CB_UIManager.h"
#include "InputActionValue.h"
#include "CB_PlayerWidgetContainer.h"
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

    UPROPERTY()
    UCB_UIManager* UIManager;
    UFUNCTION(BlueprintCallable, Category = "UI")
    UCB_UIManager* GetUIManager() const;

    UFUNCTION(Server, Reliable)
    void ServerStartGame();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastStartGame();

    UFUNCTION(Client, Reliable)
    void ClientCreatePlayerInfoUI();

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

    UFUNCTION()
    void AttackPunch(const FInputActionValue& Value);

    UFUNCTION()
    void AttackKick(const FInputActionValue& Value);

    UFUNCTION(Server, Reliable)
    void ServerStartDash(FVector StartLocation, FVector TargetLocation);


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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* DropDownAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCB_PlayerWidgetContainer> PlayerWidgetContainerClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* PunchAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* KickAction;

    void HandleDropDownInput(const FInputActionValue& Value);
    
private:
    FVector DashStartLocation;
    FVector DashTargetLocation;
    float DashInterpAlpha = 0.f;
    float DashSpeed = 2500.f;
    float DashDistance = 400.f;

    bool bCanDash = true;
    bool bIsDashing = false;
    FTimerHandle DashCooldownHandle;  // 쿨타임 타이머

    UCB_PlayerWidgetContainer* PlayerWidgetContainer;

};
