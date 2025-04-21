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

void UCB_UIManager::ExitGame()
{
	if (OwningController)
	{
		UKismetSystemLibrary::QuitGame(OwningController, OwningController, EQuitPreference::Quit, true);
	}
}