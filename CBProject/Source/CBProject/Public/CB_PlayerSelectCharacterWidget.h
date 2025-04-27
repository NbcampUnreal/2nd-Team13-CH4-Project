// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CB_PlayerSelectCharacterWidget.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class ACB_PlayerState;


UCLASS()
class CBPROJECT_API UCB_PlayerSelectCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TeamIndexText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NameText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* CharacterImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* SlotBorder;
	
	UPROPERTY(BlueprintReadWrite, Category = "Player")
	ACB_PlayerState* PlayerState;
	
	UFUNCTION()
	void SetPlayerName(ACB_PlayerState* NewPlayerState);
	UFUNCTION()
	void SetPlayerTeam(ACB_PlayerState* NewPlayerState);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetCharacterImage(UTexture2D* Texture);

private:
	static FLinearColor GetTeamColor(const int32 TeamNum);
};
