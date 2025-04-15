// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CB_PlayerState.generated.h"


UCLASS()
class CBPROJECT_API ACB_PlayerState : public APlayerState
{
	GENERATED_BODY()

public: 
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Lives = 3;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
