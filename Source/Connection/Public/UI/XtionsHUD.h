// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XtionsHUD.generated.h"

class AXtionsCharacter;
class ULevelManager;
class UOverlayWidgetController;
class UOverlayWidget;

/**
 * 
 */
UCLASS()
class CONNECTION_API AXtionsHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(AXtionsCharacter* InCharacter, ULevelManager* InLevelManager);

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> OverlayClass;

private:
	UPROPERTY()
	UOverlayWidget* Overlay;

	UPROPERTY()
	UOverlayWidgetController* OverlayController;
};
