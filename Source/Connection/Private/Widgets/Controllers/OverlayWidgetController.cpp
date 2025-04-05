// Copyright Leslie Apland


#include "Widgets/Controllers/OverlayWidgetController.h"
#include "LevelManagement/LevelManager.h"
#include "Characters/XtionsCharacter.h"

void UOverlayWidgetController::SetWidgetControllerParams(AXtionsCharacter* InCharacter, ULevelManager* InLevelManager)
{
	Character = InCharacter;
	LevelManager = InLevelManager;
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	LevelManager->OnLaunchTutorial.AddDynamic(this, &UOverlayWidgetController::DisplayTutorialText);
	LevelManager->OnCharacterTransport.AddDynamic(this, &UOverlayWidgetController::DisplayTransportText);
	LevelManager->OnConnectionMade.AddDynamic(this, &UOverlayWidgetController::DisplayConnectionText);
	LevelManager->OnLevelComplete.AddDynamic(this, &UOverlayWidgetController::DisplayLevelCompleteText);
	LevelManager->OnNewLevel.AddDynamic(this, &UOverlayWidgetController::UpdateNewLevelInfo);
	LevelManager->OnPlayerHitTooManyTimes.AddDynamic(this, &UOverlayWidgetController::DisplayEndLevel);
	Character->OnCharacterHit.AddDynamic(this, &UOverlayWidgetController::DisplayHitsText);
	Character->OnOverlappingBox.AddDynamic(this, &UOverlayWidgetController::DisplayInterationText);
	Character->OnDodgesUpdated.AddDynamic(this, &UOverlayWidgetController::DisplayDodgesText);
	Character->OnKeyBindingsNeedRebuilt.AddDynamic(this, &UOverlayWidgetController::SendRebuildKeyBindings);
	Character->OnHoveredInteractable.AddDynamic(this, &UOverlayWidgetController::DisplayHoverText);
	Character->OnPickedUpInteractable.AddDynamic(this, &UOverlayWidgetController::UpdateInteractableInfo);
	Character->OnNotReadyToRepair.AddDynamic(this, &UOverlayWidgetController::DisplayRepairNotReadyText);

}

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnInitializeHits.Broadcast();
	OnInitializeDodges.Broadcast(Character->GetStartDodges());
}

void UOverlayWidgetController::EndLevel()
{
	LevelManager->EndLevelPrematurely();
}

void UOverlayWidgetController::DisplayTutorialText()
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidgetController::DisplayTutorialText(): OnShowTutorial.Broadcast()"));
	OnShowTutorial.Broadcast();
}

void UOverlayWidgetController::DisplayHitsText()
{
	IncrementHits.Broadcast();
}

void UOverlayWidgetController::DisplayTransportText()
{
	OnShowCharacterTransportText.Broadcast();
}

void UOverlayWidgetController::DisplayConnectionText()
{
	OnShowConnectionMadeText.Broadcast();
}

void UOverlayWidgetController::DisplayLevelCompleteText(bool bLevelSuccessfullyCompleted)
{
	if (bLevelSuccessfullyCompleted)
	{
		OnShowLevelCompleteText.Broadcast();
	}
	else
	{
		OnShowLevelFailedText.Broadcast();
	}
}

void UOverlayWidgetController::DisplayInterationText(bool bOverlapping, bool bReadyToRepair)
{
	OnShowInteractionText.Broadcast(bOverlapping, bReadyToRepair);
}

void UOverlayWidgetController::DisplayDodgesText(int32 Amount)
{
	OnUpdateDodgesText.Broadcast(Amount);
}

void UOverlayWidgetController::UpdateNewLevelInfo(int32 InTotalConnectionBoxes, int32 InMaxHits, TArray<int32> InRequiredObjects, TArray<int32> InObjectCounts)
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidgetController::UpdateNewLevelInfo(): InTotalConnectionBoxes: %i, InMaxHits: %i"), InTotalConnectionBoxes, InMaxHits);
	OnShowNewLevelInfo.Broadcast(InTotalConnectionBoxes, InMaxHits, InRequiredObjects, InObjectCounts);
}

void UOverlayWidgetController::SendRebuildKeyBindings()
{
	OnRebuildKeyBindings.Broadcast();
}

void UOverlayWidgetController::DisplayHoverText(FString HoverText)
{
	OnUpdateHoverText.Broadcast(HoverText);
}

void UOverlayWidgetController::UpdateInteractableInfo(int32 InID, int32 CurrentCount, int32 TotalCount, bool InShowEnabled)
{
	OnUpdateInteractableInfo.Broadcast(InID, CurrentCount, TotalCount, InShowEnabled);
}

void UOverlayWidgetController::DisplayRepairNotReadyText()
{
	OnDisplayRepairNotReadyText.Broadcast();
}

void UOverlayWidgetController::DisplayEndLevel()
{
	OnLevelIsOver.Broadcast();
}
