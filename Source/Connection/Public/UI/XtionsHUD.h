// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XtionsHUD.generated.h"

class AXtionsCharacter;
class ULevelManager;
class UOverlayWidgetController;
class UOverlayWidget;
class ULevelSelection;
class UMissionDescription;

/**
 * 
 */
UCLASS()
class CONNECTION_API AXtionsHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(AXtionsCharacter* InCharacter, ULevelManager* InLevelManager);
	void SetShowOverlay(bool bShowOverlay);
	void ShowLevelSelectionScreen();

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> OverlayClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> LevelSelectionClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> PreGameMissionDescClass;

private:
	UPROPERTY()
	UOverlayWidget* Overlay;

	UPROPERTY()
	UOverlayWidgetController* OverlayController;

	UPROPERTY()
	AXtionsCharacter* Character;

	UPROPERTY()
	ULevelSelection* SelectLevelScreen;

	UPROPERTY()
	UMissionDescription* MissionDescription;

	bool bOverlayIsShown = false;
};
