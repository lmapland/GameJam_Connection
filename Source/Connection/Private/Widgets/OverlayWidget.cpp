// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverlayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Widgets/Controllers/OverlayWidgetController.h"

/*void UOverlayWidget::SetLives(int32 DefaultLives)
{
	CurrentLives = DefaultLives;

	for (int32 i = 0; i < DefaultLives; i++)
	{
		UProgressBar* Bar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
		LivesBox->AddChildToUniformGrid(Bar, 0, i);
	}
}*/

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

void UOverlayWidget::NativeConstruct()
{
	Section1->SetVisibility(ESlateVisibility::Hidden);
	Section2->SetVisibility(ESlateVisibility::Hidden);
	Section3->SetVisibility(ESlateVisibility::Hidden);
	Section4->SetVisibility(ESlateVisibility::Hidden);
	InteractionText->SetVisibility(ESlateVisibility::Hidden);
	DodgesAnimText->SetVisibility(ESlateVisibility::Hidden);
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
	WidgetController->OnShowInteractionText.AddDynamic(this, &UOverlayWidget::DisplayInteractionText);
	WidgetController->OnInitializeDodges.AddDynamic(this, &UOverlayWidget::DisplayDodgesText);
	WidgetController->OnUpdateDodgesText.AddDynamic(this, &UOverlayWidget::DisplayDodgesText);
}

void UOverlayWidget::DisplayConnectionText()
{
	if (Section1 && !GameOver)
	{
		Section1->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section1Handle, this, &UOverlayWidget::HideSection1Text, ConnectionTime);
	}
}

void UOverlayWidget::DisplayLevelCompleteText()
{
	if (Section3 && !GameOver)
	{
		Section3->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section3Handle, this, &UOverlayWidget::HideSection3Text, LevelCompleteTime);
	}
}

void UOverlayWidget::DisplayTransportingText()
{
	if (Section2 && Section2Text)
	{
		Section2Text->SetText(FText::FromString("Transporting to next Level, please wait..."));
		Section2->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::HideSection2Text, TransportingTime);

	}
}

void UOverlayWidget::DisplayTutorialText()
{
	if (Section2)
	{
		Section2->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::DisplayTutorialPart2Text, TutorialTime);
	}
}

void UOverlayWidget::DisplayTutorialPart2Text()
{
	if (Section2)
	{
		Section2Text->SetText(FText::FromString("Press 'r' to dodge"));
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::HideSection2Text, TutorialTime);
	}
}

void UOverlayWidget::DisplayDeathText()
{
	GameOver = true;
	if (Section4)
	{
		Section4->SetVisibility(ESlateVisibility::Visible);
	}
	if (Section1 && Section2 && Section3)
	{
		Section1->SetVisibility(ESlateVisibility::Hidden);
		Section2->SetVisibility(ESlateVisibility::Hidden);
		Section3->SetVisibility(ESlateVisibility::Hidden);
	}
	GetWorld()->GetTimerManager().SetTimer(Section4Handle, this, &UOverlayWidget::HideSection4Text, DeathTime);
}

void UOverlayWidget::DisplayWinText()
{
	GameOver = true;
	if (Section4 && Section4Text)
	{
		Section4Text->SetText(FText::FromString("You Won!"));
		Section4->SetVisibility(ESlateVisibility::Visible);
	}
	if (Section1 && Section2 && Section3)
	{
		Section1->SetVisibility(ESlateVisibility::Hidden);
		Section2->SetVisibility(ESlateVisibility::Hidden);
		Section3->SetVisibility(ESlateVisibility::Hidden);
	}
	GetWorld()->GetTimerManager().SetTimer(Section4Handle, this, &UOverlayWidget::HideSection4Text, WinTime);
}

void UOverlayWidget::DisplayInteractionText(bool bIsVisible)
{
	if (InteractionText)
	{
		if (bIsVisible)	InteractionText->SetVisibility(ESlateVisibility::Visible);
		else InteractionText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::DisplayDodgesText(int32 Dodges)
{
	// Play "you gained dodges" animation
	if (CurrentDodges != -1)
	{
		int32 GainedDodges = Dodges - CurrentDodges;
		if (GainedDodges > 0)
		{
			FString NewDodgesText = FString::Printf(TEXT("+%i"), GainedDodges);
			DodgesAnimText->SetText(FText::FromString(NewDodgesText));
			// play anim
			PlayAnimation(NewDodgeAnimation);
		}
	}

	if (DodgesText)
	{
		FString DodgesString = FString::Printf(TEXT("Dodges: %i"), Dodges);
		DodgesText->SetText(FText::FromString(DodgesString));
	}

	CurrentDodges = Dodges;
}

void UOverlayWidget::HideSection1Text()
{
	if (Section1)
	{
		Section1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::HideSection2Text()
{
	if (Section2)
	{
		Section2->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::HideSection3Text()
{
	if (Section3)
	{
		Section3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::HideSection4Text()
{
	if (Section4)
	{
		Section4->SetVisibility(ESlateVisibility::Hidden);
	}
}
