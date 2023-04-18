// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelManager.generated.h"

class AStartZone;
class AXtionsCharacter;

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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLevel = 1;

	int32 TotalLevels = 6;

	bool bCurrentLevelStarted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<int32> Levels;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> StartingLocations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	TArray<FRotator> StartingRotations;

	int32 Progress = 0; // if the current level has 2 ConnectionBoxes, then this can either be 0 or 1

	TObjectPtr<AXtionsCharacter> Owner;

};
