// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DynamicCameraActor.generated.h"

UCLASS()
class CBPROJECT_API ADynamicCameraActor : public AActor
{
	GENERATED_BODY()
	
public:
	ADynamicCameraActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
};
