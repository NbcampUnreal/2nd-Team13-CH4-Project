#include "CB_CharacterSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UCB_CharacterSlot::SetPlayerName(const FString& InName)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(InName));
	}
}

void UCB_CharacterSlot::SetCharacterImage(UTexture2D* Texture)
{
	if (CharacterImage && Texture)
	{
		CharacterImage->SetBrushFromTexture(Texture);
	}
}

void UCB_CharacterSlot::SetWeaponImage(UTexture2D* Texture)
{
	if (WeaponImage && Texture)
	{
		WeaponImage->SetBrushFromTexture(Texture);
	}
}

void UCB_CharacterSlot::SetSlotColor(const FLinearColor& Color)
{
	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(Color);
	}
}