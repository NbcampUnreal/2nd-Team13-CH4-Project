#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CB_FigtherCharacter.generated.h"

UCLASS()
class CBPROJECT_API ACB_FigtherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACB_FigtherCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
