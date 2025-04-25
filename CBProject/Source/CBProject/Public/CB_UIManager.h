#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CB_UIManager.generated.h"


UCLASS(Blueprintable)
class CBPROJECT_API UCB_UIManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(class APlayerController* InOwningController);

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable)
	void ShowSelectMenu();
	UFUNCTION(BlueprintCallable)
	void ShowSettings();
	UFUNCTION(BlueprintCallable)
	void CharaterSelect();
	UFUNCTION(BlueprintCallable)
	void StartGame();
	UFUNCTION(BlueprintCallable)
	void UpdateCountdown(int32 CountdownTime);
	UFUNCTION(BlueprintCallable)
	void ShowCountdownWidget();
	UFUNCTION(BlueprintCallable)
	void RemoveCountdownWidget();
	UFUNCTION(BlueprintCallable)
	void ExitGame();

protected:
	UPROPERTY()
	APlayerController* OwningController;
	UPROPERTY()
	UUserWidget* CurrentWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SelectMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SettingMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CharacterSelectMenuClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CountdownWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BattleUI;

	UUserWidget* CountdownWidget = nullptr;


	void SwitchToWidget(TSubclassOf<UUserWidget> WidgetClass);
};
