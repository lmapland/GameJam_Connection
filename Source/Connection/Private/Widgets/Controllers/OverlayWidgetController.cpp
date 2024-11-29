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
	LevelManager->OnNewLevelSignature.AddDynamic(this, &UOverlayWidgetController::UpdateTotalLevels);
	Character->OnCharacterHit.AddDynamic(this, &UOverlayWidgetController::DisplayHitsText);
	Character->OnOverlappingBox.AddDynamic(this, &UOverlayWidgetController::DisplayInterationText);
	Character->OnDodgesUpdated.AddDynamic(this, &UOverlayWidgetController::DisplayDodgesText);
	Character->OnJumpsUpdated.AddDynamic(this, &UOverlayWidgetController::DisplayJumpsText);
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnInitializeHits.Broadcast();
	OnInitializeDodges.Broadcast(Character->GetStartDodges());
	OnUpdateJumpsText.Broadcast(Character->GetStartJumps());
}

void UOverlayWidgetController::DisplayTutorialText()
{
	OnShowTutorial.Broadcast();
}

void UOverlayWidgetController::DisplayHitsText()
{
	IncrementHits.Broadcast();
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

void UOverlayWidgetController::DisplayJumpsText(int32 Amount)
{
	OnUpdateJumpsText.Broadcast(Amount);
}

void UOverlayWidgetController::UpdateTotalLevels(int32 TotalLevels)
{
	//UE_LOG(LogTemp, Warning, TEXT("UOverlayWidgetController::UpdateTotalLevels(): TotalLevels: %i"), TotalLevels);
	OnUpdateTotalLevels.Broadcast(TotalLevels);
}
