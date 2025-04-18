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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInitializeLevelStats, TArray<int32>, NumCompletions, TArray<float>, CompletionTime, bool, bFinalLevelUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnLevelStatsUpdated, int32, CurrentLevel, int32, NumCompletions, float, CompletionTime, bool, bFinalLevelUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartLevel, int32, InCurrentLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerHitTooManyTimes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThreeLevelsComplete, bool, bOnInitialize);

class AStartZone;
class AXtionsCharacter;
class AConnectionBox;

/**
 * To use this level manager:
 * For each level, create a StartZone and give it a Level equal to its level AND A TAG equal to its level
 * For each Enemy, give it a tag indicating its level (it has a Level variable,  not setting it is not required)
 * For each Connection Box, give it a Level equal to its level AND A TAG equal to its level
 * For each Item, give it a Tag equal to its level
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
	void Setup(TArray<int32> InLevelCompletions, TArray<float> InLevelTimes, bool InbFinalLevelUnlocked);

	UFUNCTION()
	void SkipCurrentLevel();

	UFUNCTION(BlueprintCallable)
	void StartLevel(int32 LevelToStart);

	void TransportPlayer(int32 ToLevel);

	void EndLevelPrematurely();

	void StartFinalLevel();

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
	FOnInitializeLevelStats OnInitializeLevelStats;

	UPROPERTY()
	FOnLevelStatsUpdated OnLevelStatsUpdated;

	UPROPERTY()
	FOnStartLevel OnStartLevel;

	UPROPERTY()
	FOnPlayerHitTooManyTimes OnPlayerHitTooManyTimes;

	UPROPERTY()
	FOnThreeLevelsComplete OnThreeLevelsComplete;

private:
	bool BaseLevelsAreComplete();
	void EndTheGame();
	void TransportPlayer();
	void RestartLevel();

	UFUNCTION()
	void TearDownLevel();

	UFUNCTION()
	void PlayerHit();

	void UpdateLevelStats();
	void SetFinalLevelUnlocked(bool bOnInitialize);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevel = 1;

	int32 LastLevelPlayed = 1;

	bool bCurrentLevelStarted = false;
	
	UPROPERTY(EditAnywhere, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FLevelInfo> Levels;

	TArray<AConnectionBox*> AllConnectionBoxes;

	int32 Progress = 0; // if the current level has 2 ConnectionBoxes, then this can either be 0 or 1
	int32 CurrentHits = 0;

	TObjectPtr<AXtionsCharacter> Player;

	int32 MaxLevel = 4;

	FTimerHandle TransportTimer;
	float TransportTime = 4.f;

	/* Lowest time in seconds player took to play the level. [0] == level 1 */
	TArray<float> LevelTimes;

	/* # of completions per level */
	TArray<int32> LevelCompletions;
	bool bAllLevelsAreComplete = false;

	float InitialTime = 0.f;
	float LevelTime = 0.f;

public:
	FORCEINLINE TArray<int32> GetLevelCompletions() const { return LevelCompletions; }
	FORCEINLINE TArray<float> GetLevelTimes() const { return LevelTimes; }
	FORCEINLINE bool GetFinalLevelUnlocked() const { return bAllLevelsAreComplete; }
};
