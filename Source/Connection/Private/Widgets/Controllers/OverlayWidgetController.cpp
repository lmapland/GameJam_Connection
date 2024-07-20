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
	LevelManager->OnPlayerWin.AddDynamic(this, &UOverlayWidgetController::DisplayWinText);
	LevelManager->OnCharacterTransport.AddDynamic(this, &UOverlayWidgetController::DisplayTransportText);
	LevelManager->OnConnectionMade.AddDynamic(this, &UOverlayWidgetController::DisplayConnectionText);
	LevelManager->OnLevelComplete.AddDynamic(this, &UOverlayWidgetController::DisplayLevelCompleteText);
	Character->OnLivesUpdated.AddDynamic(this, &UOverlayWidgetController::UpdateHUDLives);
	Character->OnCharacterDeath.AddDynamic(this, &UOverlayWidgetController::DisplayDeathText);
	Character->OnOverlappingBox.AddDynamic(this, &UOverlayWidgetController::DisplayInterationText);
	Character->OnDodgesUpdated.AddDynamic(this, &UOverlayWidgetController::DisplayDodgesText);
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnInitializeLives.Broadcast(Character->GetStartLives());
	OnInitializeDodges.Broadcast(Character->GetStartDodges());
}

void UOverlayWidgetController::DisplayTutorialText()
{
	OnShowTutorial.Broadcast();
}

void UOverlayWidgetController::UpdateHUDLives(int32 Amount)
{
	OnUpdateLives.Broadcast(-1);
}

void UOverlayWidgetController::DisplayDeathText()
{
	OnShowCharacterDeathText.Broadcast();
}

void UOverlayWidgetController::DisplayWinText()
{
	OnShowPlayerWinText.Broadcast();
}

void UOverlayWidgetController::DisplayTransportText()
{
	OnShowCharacterTransportText.Broadcast();
}

void UOverlayWidgetController::DisplayConnectionText()
{
	OnShowConnectionMadeText.Broadcast();
}

void UOverlayWidgetController::DisplayLevelCompleteText()
{
	OnShowLevelCompleteText.Broadcast();
}

void UOverlayWidgetController::DisplayInterationText(bool bOverlapping)
{
	OnShowInteractionText.Broadcast(bOverlapping);
}

void UOverlayWidgetController::DisplayDodgesText(int32 Amount)
{
	OnUpdateDodgesText.Broadcast(Amount);
}
