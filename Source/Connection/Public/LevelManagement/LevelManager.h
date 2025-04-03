// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/LevelInfo.h"
#include "LevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchTutorial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterTransport);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelComplete, bool, bLevelPassed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionMade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnNewLevel, int32, InTotalConnectionBoxes, int32, InMaxHits, TArray<int32>, InRequiredObjects, TArray<int32>, InObjectCounts);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLevelStatsUpdated, int32, CurrentLevel, int32, NumCompletions, float, CompletionTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartLevel, int32, InCurrentLevel);

class AStartZone;
class AXtionsCharacter;
class AConnectionBox;

/**
 * To use this level manager:
 * For each level, create a StartZone and give it a Level equal to its level AND A TAG equal to its level
 * For each Enemy, give it a tag indicating its level (it has a Level variable,  not setting it is not required)
 * For each Connection Box, give it a Level equal to its level AND A TAG equal to its level
 * Make sure the initializers are correct in this .cpp file:
 *  the number of CBs required to complete each level;
 *  the location the character should be transported to (should be within the StartZone)
 */
UCLASS(BlueprintType)
class CONNECTION_API ULevelManager : public UObject
{
	GENERATED_BODY()
	
public:
	ULevelManager();

	UFUNCTION()
	void CharacterInPlay(int32 InLevel);

	UFUNCTION()
	void ConnectionComplete(int32 LevelOfBox);
	
	UFUNCTION(BlueprintCallable)
	void Setup(TArray<int32> InLevelCompletions, TArray<float> InLevelTimes);

	UFUNCTION()
	void SkipCurrentLevel();

	UFUNCTION(BlueprintCallable)
	void StartLevel(int32 LevelToStart);

	void TransportPlayer(int32 ToLevel);

	UPROPERTY()
	FOnLaunchTutorial OnLaunchTutorial;

	UPROPERTY()
	FOnCharacterTransport OnCharacterTransport;

	UPROPERTY()
	FOnLevelComplete OnLevelComplete;

	UPROPERTY()
	FOnConnectionMade OnConnectionMade;

	UPROPERTY()
	FOnNewLevel OnNewLevel;

	UPROPERTY()
	FOnLevelStatsUpdated OnLevelStatsUpdated;

	UPROPERTY()
	FOnStartLevel OnStartLevel;

private:
	void EndTheGame();
	void TransportPlayer();
	void RestartLevel();

	UFUNCTION()
	void TearDownLevel();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevel = 1;

	int32 LastLevelPlayed = 1;

	bool bCurrentLevelStarted = false;
	
	UPROPERTY(EditAnywhere, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FLevelInfo> Levels;

	TArray<AConnectionBox*> AllConnectionBoxes;

	int32 Progress = 0; // if the current level has 2 ConnectionBoxes, then this can either be 0 or 1

	TObjectPtr<AXtionsCharacter> Player;

	int32 MaxLevel = 4;

	FTimerHandle TransportTimer;
	float TransportTime = 4.f;

	/* Lowest time in seconds player took to play the level. [0] == level 1 */
	TArray<float> LevelTimes;

	/* # of completions per level */
	TArray<int32> LevelCompletions;

	float InitialTime = 0.f;
	float LevelTime = 0.f;

public:
	FORCEINLINE TArray<int32> GetLevelCompletions() const { return LevelCompletions; }
	FORCEINLINE TArray<float> GetLevelTimes() const { return LevelTimes; }
};
