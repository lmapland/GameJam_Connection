// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelection.generated.h"

class UTextBlock;
class ULevelManager;
class ULevelMissionDescription;

/**
 * 
 */
UCLASS()
class CONNECTION_API ULevelSelection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(ULevelManager* InLevelManager);

	UFUNCTION(BlueprintImplementableEvent)
	void LoadLevel();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level1NumCompletions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level2NumCompletions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level3NumCompletions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level1CompletionTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level2CompletionTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level3CompletionTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	ULevelMissionDescription* MissionDescription;

protected:
	UFUNCTION(BlueprintCallable)
	void ShowMissionDescription(int32 InLevel);

	/* Lowest time in seconds player took to play the level. [0] == level 1 */
	UPROPERTY(BlueprintReadOnly)
	TArray<float> LevelTimes;

	/* # of completions per level */
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> LevelCompletions;

	UPROPERTY(BlueprintReadWrite)
	int32 LevelSelected = -1;

private:
	UFUNCTION()
	void LevelStatsUpdated(int32 InLevel, int32 NumCompletions, float CompletionTime);

	TArray<UTextBlock*> CompletionTimeTextBlocks;

	TArray<UTextBlock*> NumCompletionTextBlocks;

};
