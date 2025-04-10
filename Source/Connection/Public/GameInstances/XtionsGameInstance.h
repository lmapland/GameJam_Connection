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
	void SetShowOverlay(bool bShowOverlay);
	void ShowLevelSelectionScreen();

	UFUNCTION(BlueprintImplementableEvent)
	TArray<int32> GetLevelCompletions();

	UFUNCTION(BlueprintImplementableEvent)
	TArray<float> GetLevelTimes();

	UFUNCTION(BlueprintImplementableEvent)
	bool GetFinalLevelUnlocked();

	UFUNCTION(BlueprintImplementableEvent)
	void SetLevelStats(const TArray<int32>& InLevelCompletions, const TArray<float>& InLevelTimes, bool bFinalLevelUnlocked);

private:
	UFUNCTION()
	void LevelStatsUpdated(int32 InLevel, int32 NumCompletions, float CompletionTime, bool bFinalLevelUnlocked);

	UPROPERTY()
	ULevelManager* LevelManager;

	UPROPERTY()
	AXtionsCharacter* Character;

	UPROPERTY()
	APlayerController* Controller;

	UPROPERTY()
	AXtionsHUD* HUD;

	TArray<int32> LevelCompletions;

	TArray<float> LevelTimes;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ULevelManager* GetLevelManager() const { return LevelManager; }
};
