// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchTutorialSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerWinSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterTransportSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelCompleteSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionMadeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewLevelSignature, int32, TotalLevels);

class AStartZone;
class AXtionsCharacter;
class AConnectionBox;

/**
 * To use this level manager:
 * For each level, create a StartZone and give it a Level equal to its level (no tags necessary)
 * For each Enemy, give it a tag indicating its level
 * For each Connection Box, give it a Level equal to its level (no tags necessary)
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
	void Setup();

	UFUNCTION()
	void SkipCurrentLevel();

	UPROPERTY()
	FOnLaunchTutorialSignature OnLaunchTutorial;
	
	UPROPERTY()
	FOnPlayerWinSignature OnPlayerWin;

	UPROPERTY()
	FOnCharacterTransportSignature OnCharacterTransport;

	UPROPERTY()
	FOnLevelCompleteSignature OnLevelComplete;

	UPROPERTY()
	FOnConnectionMadeSignature OnConnectionMade;

	UPROPERTY()
	FOnNewLevelSignature OnNewLevelSignature;

private:
	void EndTheGame();
	void TransportPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevel = 1;

	bool bCurrentLevelStarted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<int32> Levels;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> StartingLocations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FRotator> StartingRotations;

	TArray<AConnectionBox*> AllConnectionBoxes;

	int32 Progress = 0; // if the current level has 2 ConnectionBoxes, then this can either be 0 or 1

	TObjectPtr<AXtionsCharacter> Player;

	int32 MaxLevel = 5;

	FTimerHandle TransportTimer;
	float TransportTime = 4.f;

};
