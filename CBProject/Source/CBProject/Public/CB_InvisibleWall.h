#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CB_InvisibleWall.generated.h"

UCLASS()
class CBPROJECT_API ACB_InvisibleWall : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACB_InvisibleWall();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

};
