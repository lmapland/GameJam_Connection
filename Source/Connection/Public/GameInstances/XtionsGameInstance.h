// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "XtionsGameInstance.generated.h"

class ULevelManager;
class AXtionsCharacter;
class AXtionsHUD;
/**
 * 
 */
UCLASS()
class CONNECTION_API UXtionsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void InitHUD(AXtionsCharacter* InCharacter, APlayerController* InPlayerController);

private:
	UPROPERTY()
	ULevelManager* LevelManager;

	UPROPERTY()
	AXtionsCharacter* Character;

	UPROPERTY()
	APlayerController* Controller;

	UPROPERTY()
	AXtionsHUD* HUD;
};
