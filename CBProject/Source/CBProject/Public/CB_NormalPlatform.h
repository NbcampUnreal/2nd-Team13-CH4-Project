#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CB_NormalPlatform.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class CBPROJECT_API ACB_NormalPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACB_NormalPlatform();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Collision")
	UBoxComponent* OneWayCollisionBox;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
