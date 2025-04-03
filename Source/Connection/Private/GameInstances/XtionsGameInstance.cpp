// Copyright Leslie Apland


#include "GameInstances/XtionsGameInstance.h"
#include "LevelManagement/LevelManager.h"
#include "Characters/XtionsCharacter.h"
#include "UI/XtionsHUD.h"

void UXtionsGameInstance::InitHUD(AXtionsCharacter* InCharacter, APlayerController* InPlayerController)
{
	LevelManager = NewObject<ULevelManager>(this);
	Character = InCharacter;
	Controller = InPlayerController;
	HUD = Cast<AXtionsHUD>(Controller->GetHUD());

	HUD->InitOverlay(Character, LevelManager);
	LevelManager->OnLevelStatsUpdated.AddDynamic(this, &UXtionsGameInstance::LevelStatsUpdated);

	LevelManager->Setup(GetLevelCompletions(), GetLevelTimes());
	LevelCompletions = LevelManager->GetLevelCompletions();
	LevelTimes = LevelManager->GetLevelTimes();
	SetLevelStats(LevelCompletions, LevelTimes);
}

void UXtionsGameInstance::SetShowOverlay(bool bShowOverlay)
{
	HUD->SetShowOverlay(bShowOverlay);
}

void UXtionsGameInstance::ShowLevelSelectionScreen()
{
	HUD->ShowLevelSelectionScreen();
}

void UXtionsGameInstance::LevelStatsUpdated(int32 InLevel, int32 NumCompletions, float CompletionTime)
{
	UE_LOG(LogTemp, Warning, TEXT("UXtionsGameInstance::LevelStatsUpdated()"));
	if (InLevel > LevelCompletions.Num() || InLevel > LevelTimes.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("UXtionsGameInstance::LevelStatsUpdated(): InLevel out of bounds for arrays"));
		return;
	}
	LevelCompletions[InLevel - 1] = NumCompletions;
	LevelTimes[InLevel - 1] = CompletionTime;
	SetLevelStats(LevelCompletions, LevelTimes);
}
