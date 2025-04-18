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

	//편집 가능 오프셋
	UPROPERTY(EditAnywhere, Category = "Camera|Offset")
	float CameraYOffset = -800.f;
	UPROPERTY(EditAnywhere, Category = "Camera|Offset")
	float CameraZOffset = 300.f;

protected:
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
};