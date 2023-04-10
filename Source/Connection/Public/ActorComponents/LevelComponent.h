// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelComponent.generated.h"

class AConnectionBox;
class AXtionsCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONNECTION_API ULevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevelComponent();
	void LevelUpdate(int32 LevelOfBox);
	void Setup(AXtionsCharacter* Char);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TArray<int32> Levels;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TArray<FVector> StartingLocations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TArray<FRotator> StartingRotations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	int32 CurrentLevel = 1; // The current LEVEL. The game starts at Level 1, so use index 0

	int32 Progress = 0; // if the current level has 2 ConnectionBoxes, then this can either be 0 or 1

protected:
	virtual void BeginPlay() override;

private:
	AXtionsCharacter* Owner;
};
