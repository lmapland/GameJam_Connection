// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverlayWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Widgets/Controllers/OverlayWidgetController.h"
#include "UI/MissionItemsContainer.h"

/*void UOverlayWidget::SetLives(int32 DefaultLives)
{
	CurrentLives = DefaultLives;

	for (int32 i = 0; i < DefaultLives; i++)
	{
		UProgressBar* Bar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
		LivesBox->AddChildToUniformGrid(Bar, 0, i);
	}
}*/

void UOverlayWidget::SetController(UOverlayWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;

	WidgetController->OnShowNewLevelInfo.AddDynamic(this, &UOverlayWidget::DisplayNewLevelInfo);
	WidgetController->OnInitializeHits.AddDynamic(this, &UOverlayWidget::SetHits);
	WidgetController->OnShowTutorial.AddDynamic(this, &UOverlayWidget::DisplayTutorialText);
	WidgetController->IncrementHits.AddDynamic(this, &UOverlayWidget::UpdateHits);
	WidgetController->OnShowCharacterTransportText.AddDynamic(this, &UOverlayWidget::DisplayTransportingText);
	WidgetController->OnShowLevelCompleteText.AddDynamic(this, &UOverlayWidget::DisplayLevelCompleteText);
	WidgetController->OnShowLevelFailedText.AddDynamic(this, &UOverlayWidget::DisplayLevelFailedText);
	WidgetController->OnShowConnectionMadeText.AddDynamic(this, &UOverlayWidget::DisplayConnectionText);
	WidgetController->OnShowInteractionText.AddDynamic(this, &UOverlayWidget::DisplayInteractionText);
	WidgetController->OnInitializeDodges.AddDynamic(this, &UOverlayWidget::DisplayDodgesText);
	WidgetController->OnUpdateDodgesText.AddDynamic(this, &UOverlayWidget::DisplayDodgesText);
	WidgetController->OnRebuildKeyBindings.AddDynamic(this, &UOverlayWidget::RebuildKeyBindings);
	WidgetController->OnUpdateHoverText.AddDynamic(this, &UOverlayWidget::DisplayHoverText);
	WidgetController->OnUpdateInteractableInfo.AddDynamic(this, &UOverlayWidget::UpdateInteractableInfo);
	WidgetController->OnDisplayRepairNotReadyText.AddDynamic(this, &UOverlayWidget::DisplayRepairNotReadyText);
}

void UOverlayWidget::DisplayConnectionText()
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayConnectionText()"));
	CurrentBox += 1;
	if (BoxesText)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayConnectionText(): CurrentBox: %i"), CurrentBox);
		FString BoxesString = FString::Printf(TEXT("%i/%i"), CurrentBox, TotalBoxes);
		BoxesText->SetText(FText::FromString(BoxesString));
	}

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
		Section3Text->SetText(FText::FromString("Level Complete!"));
		Section3->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section3Handle, this, &UOverlayWidget::HideSection3Text, LevelCompleteTime);
	}
}

void UOverlayWidget::DisplayLevelFailedText()
{
	if (Section3 && !GameOver)
	{
		Section3Text->SetText(FText::FromString("You were hit too many times! Resetting level for retry"));
		Section3->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section3Handle, this, &UOverlayWidget::HideSection3Text, LevelCompleteTime);
	}
}

void UOverlayWidget::DisplayTransportingText()
{
	if (Section2 && Section2Text && Hits <= MaxHits)
	{
		Section2Text->SetText(FText::FromString("Transporting to next Level, please wait..."));
		Section2->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::HideSection2Text, TransportingTime);
	}
}

void UOverlayWidget::DisplayTutorialText()
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayTutorialText()"));
	if (Section2)
	{
		Section2->SetVisibility(ESlateVisibility::Visible);
		Section2Text->SetText(FText::FromString("Fix Defenses to kill the enemies"));
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::HideSection2Text, TutorialTime);
	}
}

void UOverlayWidget::UpdateHits()
{
	Hits++;
	PlayerHurt();
	SetHits();
}

void UOverlayWidget::DisplayInteractionText(bool bIsVisible)
{
	if (InteractionHorizBox)
	{
		if (bIsVisible)	InteractionHorizBox->SetVisibility(ESlateVisibility::Visible);
		else InteractionHorizBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayWidget::DisplayDodgesText(int32 Dodges)
{
	// Play "you gained dodges" animation
	/*if (CurrentDodges != -1)
	{
		int32 GainedDodges = Dodges - CurrentDodges;
		if (GainedDodges > 0)
		{
			FString NewDodgesText = FString::Printf(TEXT("+%i"), GainedDodges);
			DodgesAnimText->SetText(FText::FromString(NewDodgesText));
			PlayAnimation(NewDodgeAnimation);
		}
	}*/

	if (Dodges == 0 && CurrentDodges == 0)
	{
		FString NoDodgesLeftString = FString::Printf(TEXT("No Dodges left!"));
		Section2Text->SetText(FText::FromString(NoDodgesLeftString));
		Section2->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::HideSection2Text, PlayerErrorTime);
	}
	else
	{
		FString DodgesString = FString::Printf(TEXT("%i"), Dodges);
		DodgesText->SetText(FText::FromString(DodgesString));
		CurrentDodges = Dodges;
	}
}

//void UOverlayWidget::DisplayJumpsText(int32 Jumps)
//{
//	if (CurrentJumps != -1)
//	{
//		int32 GainedJumps = Jumps - CurrentJumps;
//		if (GainedJumps > 0)
//		{
//			FString NewJumpsText = FString::Printf(TEXT("+%i"), GainedJumps);
//			JumpsAnimText->SetText(FText::FromString(NewJumpsText));
//			PlayAnimation(NewJumpAnimation);
//		}
//	}
//
//	if (JumpsText)
//	{
//		FString JumpsString = FString::Printf(TEXT("Jumps: %i"), Jumps);
//		JumpsText->SetText(FText::FromString(JumpsString));
//	}
//
//	CurrentJumps = Jumps;
//}

void UOverlayWidget::DisplayNewLevelInfo(int32 InTotalConnectionBoxes, int32 InMaxHits, TArray<int32> InRequiredObjects, TArray<int32> InObjectCounts)
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayNewLevelInfo()"));
	// This function is only called when a new level is started
	CurrentBox = 0;
	TotalBoxes = InTotalConnectionBoxes;
	Hits = 0;
	MaxHits = InMaxHits;
	HitsUnderMaximum();

	if (BoxesText)
	{
		FString BoxesString = FString::Printf(TEXT("%i/%i"), CurrentBox, TotalBoxes);
		BoxesText->SetText(FText::FromString(BoxesString));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayNewLevelInfo(): BoxesText is null"));
	}

	SetHits();

	MissionItemsContainer->Setup(InRequiredObjects, InObjectCounts);
}

void UOverlayWidget::RebuildKeyBindings()
{
	RebuildCustomInputMappings();
	SetKeyBindIcons();
}

void UOverlayWidget::DisplayHoverText(FString HoverText)
{
	if (HoverText != FString())
	{
		HoverName->SetText(FText::FromString(HoverText));
	}
	else
	{
		HoverName->SetText(FText::GetEmpty());
	}
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayHoverText(): Displayname: %s"), *HoverText);
}

void UOverlayWidget::UpdateInteractableInfo(int32 InID, int32 InCount, bool InShowEnabled)
{
	UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::UpdateInteractableInfo(): ID: %i, Count: %i"), InID, InCount);
	MissionItemsContainer->UpdateItem(InID, InCount, InShowEnabled);
}

void UOverlayWidget::DisplayRepairNotReadyText()
{
	// Choose which place the text will be displayed
	UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayRepairNotReadyText()"));
}

void UOverlayWidget::SetHits()
{
	if (HitsText)
	{
		if (Hits > MaxHits)
		{
			HitsOverMaximum();
		}
		FString HitsString = FString::Printf(TEXT("%i/%i"), Hits, MaxHits);
		HitsText->SetText(FText::FromString(HitsString));
	}
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
