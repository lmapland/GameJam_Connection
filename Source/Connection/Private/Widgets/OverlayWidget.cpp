// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverlayWidget.h"
#include "Components/TextBlock.h"

void UOverlayWidget::DisplayConnectionText()
{
	if (ConnectionText)
	{
		ConnectionText->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(ConnectTimer, this, &UOverlayWidget::HideConnectionText, TextClearedTime);
	}
}
void UOverlayWidget::HideConnectionText()
{
	if (ConnectionText)
	{
		ConnectionText->SetVisibility(ESlateVisibility::Hidden);
	}
}

/* Doesn't display which level, maybe have function to update the level, then this can rebuild the level string and set it before displaying? */
void UOverlayWidget::DisplayLevelCompleteText()
{
	if (LevelCompleteText)
	{
		LevelCompleteText->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(LevelTimer, this, &UOverlayWidget::HideLevelCompleteText, TextClearedTime);
	}
}
void UOverlayWidget::HideLevelCompleteText()
{
	if (LevelCompleteText)
	{
		LevelCompleteText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::DisplayTransportingText()
{
	if (TransportingText)
	{
		TransportingText->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TransportTimer, this, &UOverlayWidget::HideTransportingText, TransportTime);
	}
}
void UOverlayWidget::HideTransportingText()
{
	if (TransportingText)
	{
		TransportingText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::DisplayTutorialText()
{
	if (TutorialText)
	{
		TutorialText->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(TutorialTimer, this, &UOverlayWidget::HideTutorialText, TextClearedTime);
	}
}
void UOverlayWidget::HideTutorialText()
{
	if (TutorialText)
	{
		TutorialText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::DisplayDeathText()
{
	if (DeathText)
	{
		HideConnectionText();
		HideLevelCompleteText();
		HideTransportingText();
		HideTutorialText();
		DeathText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UOverlayWidget::DisplayWinText()
{
	if (DeathText)
	{
		HideConnectionText();
		HideLevelCompleteText();
		HideTransportingText();
		HideTutorialText();
		DeathText->SetText(FText::FromString("Congratulations, you won!"));
		DeathText->SetVisibility(ESlateVisibility::Visible);
	}
}

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
