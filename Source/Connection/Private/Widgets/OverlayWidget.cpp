// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverlayWidget.h"
#include "Components/TextBlock.h"
#include "Widgets/Controllers/OverlayWidgetController.h"

void UOverlayWidget::UpdateLives(int32 Amount)
{
	if (Amount < 0)
	{
		PlayerHurt();
	}
	else
	{
		PlayerGainedHealth();
	}
}

void UOverlayWidget::SetController(UOverlayWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;

	WidgetController->OnInitializeLives.AddDynamic(this, &UOverlayWidget::SetLives);
	WidgetController->OnShowTutorial.AddDynamic(this, &UOverlayWidget::DisplayTutorialText);
	WidgetController->OnUpdateLives.AddDynamic(this, &UOverlayWidget::UpdateLives);
	WidgetController->OnShowCharacterDeathText.AddDynamic(this, &UOverlayWidget::DisplayDeathText);
	WidgetController->OnShowPlayerWinText.AddDynamic(this, &UOverlayWidget::DisplayWinText);
	WidgetController->OnShowCharacterTransportText.AddDynamic(this, &UOverlayWidget::DisplayTransportingText);
	WidgetController->OnShowLevelCompleteText.AddDynamic(this, &UOverlayWidget::DisplayLevelCompleteText);
	WidgetController->OnShowConnectionMadeText.AddDynamic(this, &UOverlayWidget::DisplayConnectionText);
}
