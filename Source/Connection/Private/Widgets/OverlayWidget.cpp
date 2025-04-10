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
	WidgetController->OnUpdateDashesText.AddDynamic(this, &UOverlayWidget::DisplayDashesText);
	WidgetController->OnRebuildKeyBindings.AddDynamic(this, &UOverlayWidget::RebuildKeyBindings);
	WidgetController->OnUpdateHoverText.AddDynamic(this, &UOverlayWidget::DisplayHoverText);
	WidgetController->OnUpdateInteractableInfo.AddDynamic(this, &UOverlayWidget::UpdateInteractableInfo);
	WidgetController->OnDisplayRepairNotReadyText.AddDynamic(this, &UOverlayWidget::DisplayRepairNotReadyText);
	WidgetController->OnLevelIsOver.AddDynamic(this, &UOverlayWidget::LaunchMissionFailedScreen);
	WidgetController->OnUpdateIntraMissionText.AddDynamic(this, &UOverlayWidget::UpdateIntraMissionText);
	WidgetController->OnDisplayDashInfo.AddDynamic(this, &UOverlayWidget::LaunchDashInfo);
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
		UpdateIntraMissionText(2, false);
		bNoUpdateMissionText = true;
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

void UOverlayWidget::DisplayInteractionText(bool bIsVisible, bool bReadyToRepair)
{
	if (InteractionHorizBox)
	{
		if (bIsVisible)
		{
			if (bReadyToRepair)
			{
				//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayInteractionText(): bReadyToRepair is true"));
				InteractionText->SetText(FText::FromString("Repair Defenses"));
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayInteractionText(): bReadyToRepair is false"));
				InteractionText->SetText(FText::FromString("Not enough items to repair!"));
			}
			InteractionHorizBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InteractionHorizBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UOverlayWidget::DisplayDodgesText(int32 Dodges)
{
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

void UOverlayWidget::DisplayDashesText(int32 Dashes)
{
	if (Dashes == 0 && CurrentDashes == 0)
	{
		FString NoDashesLeftString = FString::Printf(TEXT("No Dashes left!"));
		Section2Text->SetText(FText::FromString(NoDashesLeftString));
		Section2->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(Section2Handle, this, &UOverlayWidget::HideSection2Text, PlayerErrorTime);
	}
	else
	{
		FString DashesString = FString::Printf(TEXT("%i"), Dashes);
		DashesText->SetText(FText::FromString(DashesString));
		CurrentDashes = Dashes;
	}
}

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

void UOverlayWidget::UpdateInteractableInfo(int32 InID, int32 CurrentCount, int32 TotalCount, bool InShowEnabled)
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::UpdateInteractableInfo(): ID: %i, Count: %i"), InID, InCount);
	MissionItemsContainer->UpdateItem(InID, CurrentCount, TotalCount, InShowEnabled);
}

void UOverlayWidget::LaunchMissionFailedScreen()
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidget::DisplayMissionFailedScreen()"));
	DisplayMissionFailedScreen(this);
}

void UOverlayWidget::LaunchDashInfo(bool bOnInitialize)
{
	DisplayDashInfo(this, bOnInitialize);
}

void UOverlayWidget::UpdateIntraMissionText(int32 InMissionState, bool bInNewLevel)
{
	if (bNoUpdateMissionText && !bInNewLevel)
	{
		return;
	}
	else
	{
		bNoUpdateMissionText = false;
	}

	switch (InMissionState)
	{
	case 0:
		IntraMissionText->SetText(FText::FromString("Gather required items to repair the fence"));
		break;
	case 1:
		IntraMissionText->SetText(FText::FromString("Repair the fence"));
		break;
	case 2:
		IntraMissionText->SetText(FText::FromString(""));
		break;
	}
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

void UOverlayWidget::EndLevel()
{
	WidgetController->EndLevel();
}

void UOverlayWidget::StartFinalLevel()
{
	WidgetController->StartFinalLevel();
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
