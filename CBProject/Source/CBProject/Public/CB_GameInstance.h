// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CB_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CBPROJECT_API UCB_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "GameMode")
	int32 GameSelect = 0; //0 = 싱글, 1 = 멀티, 2 = 협동
};
