// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_PlayerSelectCharacterWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "CB_PlayerState.h"

void UCB_PlayerSelectCharacterWidget::SetPlayerName(ACB_PlayerState* NewPlayerState)
{
	UE_LOG(LogTemp, Display, TEXT("widget - SetPlayerName"));
	PlayerState = NewPlayerState;

	if (!PlayerState)
	{
		return;
	}
	
	if (NameText)
	{
		FString PlayerName = PlayerState->PlayerName;
		NameText->SetText(FText::FromString(PlayerName));
	}

	SetPlayerTeam(PlayerState);
}

void UCB_PlayerSelectCharacterWidget::SetCharacterImage(UTexture2D* Texture)
{
	if (CharacterImage && Texture)
	{
		CharacterImage->SetBrushFromTexture(Texture);
	}
}

void UCB_PlayerSelectCharacterWidget::SetPlayerTeam(ACB_PlayerState* NewPlayerState)
{
	PlayerState = NewPlayerState;

	if (!PlayerState)
	{
		return;
	}

	if (TeamIndexText)
	{
		FString TeamStr = FString::Printf(TEXT("Team %d"), PlayerState->TeamIndex);
		TeamIndexText->SetText(FText::FromString(TeamStr));
	}

	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(GetTeamColor(PlayerState->TeamIndex));
	}
}

FLinearColor UCB_PlayerSelectCharacterWidget::GetTeamColor(const int32 TeamNum)
{
	switch (TeamNum)
	{
	case 0: return FLinearColor::Red;
	case 1: return FLinearColor::Blue;
	case 2: return FLinearColor::Green;
	case 3: return FLinearColor::Yellow;
	default: return FLinearColor::White;
	}
}
