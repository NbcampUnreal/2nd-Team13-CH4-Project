#include "CB_UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UCB_UIManager::Initialize(APlayerController* InOwningController)
{
	OwningController = InOwningController;
}

void UCB_UIManager::SwitchToWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (WidgetClass && OwningController)
	{	
		CurrentWidget = CreateWidget(OwningController, WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			OwningController->SetInputMode(InputMode);
			OwningController->bShowMouseCursor = true;
		}
	}
}

void UCB_UIManager::ShowMainMenu()
{
	SwitchToWidget(MainMenuClass);
}

void UCB_UIManager::ShowSelectMenu()
{
	SwitchToWidget(SelectMenuClass);
}

void UCB_UIManager::ShowSettings()
{
	SwitchToWidget(SettingMenuClass);
}

void UCB_UIManager::CharaterSelect()
{
	SwitchToWidget(CharacterSelectMenuClass);
}

void UCB_UIManager::StartGame()
{	
	SwitchToWidget(BattleUI);
}

void UCB_UIManager::ShowCountdownWidget()
{
	if (!CountdownWidgetClass || !OwningController)
		return;

	if (CountdownWidget)
	{
		CountdownWidget->RemoveFromParent();
		CountdownWidget = nullptr;
	}

	CountdownWidget = CreateWidget<UUserWidget>(OwningController, CountdownWidgetClass);
	if (CountdownWidget)
	{
		CountdownWidget->AddToViewport();
	}
}

void UCB_UIManager::UpdateCountdown(int32 CountdownTime)
{
	if (!CountdownWidget) return;

	FName TextureKey;
	switch (CountdownTime)
	{
	case 3: TextureKey = TEXT("Texture_3"); break;
	case 2: TextureKey = TEXT("Texture_2"); break;
	case 1: TextureKey = TEXT("Texture_1"); break;
	case 0: TextureKey = TEXT("Texture_Fight"); break;
	default: return;
	}

	UFunction* SetImageFunc = CountdownWidget->FindFunction(FName("SetCountdownImageByName"));
	if (SetImageFunc)
	{
		struct FSetImageParam { FName ImageKey; };
		FSetImageParam Params{ TextureKey };
		CountdownWidget->ProcessEvent(SetImageFunc, &Params);
	}
}

void UCB_UIManager::RemoveCountdownWidget()
{
	if (CountdownWidget)
	{
		CountdownWidget->RemoveFromParent();
		CountdownWidget = nullptr;
	}

	if (PlayerWidgetContainerClass) // HPUIClass는 UPROPERTY로 받아온다고 가정
	{
		UUserWidget* HPUI = CreateWidget<UUserWidget>(OwningController, PlayerWidgetContainerClass);
		if (HPUI)
		{
			HPUI->AddToViewport();

			UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetContainerClass AddToViewport"));
		}
	}
	

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->bShowMouseCursor = false;
}

void UCB_UIManager::ExitGame()
{
	if (OwningController)
	{
		UKismetSystemLibrary::QuitGame(OwningController, OwningController, EQuitPreference::Quit, true);
	}
}