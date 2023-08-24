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
	LevelManager->Setup();
}
