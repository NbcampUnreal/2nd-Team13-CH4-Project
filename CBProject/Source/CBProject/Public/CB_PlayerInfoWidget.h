// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "CB_PlayerState.h"
#include "CB_PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class CBPROJECT_API UCB_PlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TeamIndexText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthProgress;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StockText;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	ACB_PlayerState* PlayerState;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayerState(ACB_PlayerState* NewPlayerState);

};
