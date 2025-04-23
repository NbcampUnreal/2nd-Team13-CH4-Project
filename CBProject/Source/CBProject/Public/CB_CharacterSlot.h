#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CB_CharacterSlot.generated.h"

UCLASS()
class CBPROJECT_API UCB_CharacterSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Character Slot")
	void SetPlayerName(const FString& InName);
	UFUNCTION(BlueprintCallable, Category = "Character Slot")
	void SetCharacterImage(UTexture2D* Texture);
	UFUNCTION(BlueprintCallable, Category = "Character Slot")
	void SetWeaponImage(UTexture2D* Texture);
	UFUNCTION(BlueprintCallable, Category = "Character Slot")
	void SetSlotColor(const FLinearColor& Color);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponImage;
	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotBorder;
};
