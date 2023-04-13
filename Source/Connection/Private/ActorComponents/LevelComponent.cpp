// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/LevelComponent.h"
#include "Characters/XtionsCharacter.h"

ULevelComponent::ULevelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Index 0 - Level 1
	Levels.Add(1); // To finish level 1 (0th slot), only 1 ConnectionBox needs to be connected
	StartingLocations.Add(FVector(0.f)); // Index 0 should be empty as the character starts, for level 1, at the location of the Player Start
	StartingRotations.Add(FRotator(0.f));

	// Index 1 - Level 2
	Levels.Add(1);
	StartingLocations.Add(FVector(19530.f, -270.f, 5978.f));
	StartingRotations.Add(FRotator(0.f, -40.f, 0.f));

	// Index 2 - Level 3
	Levels.Add(2);
	StartingLocations.Add(FVector(17660.f, 2570.f, 5978.f));
	StartingRotations.Add(FRotator(0.f, -90.f, 0.f));

	// Index 3 - Level 4
	Levels.Add(2);
	StartingLocations.Add(FVector(18160.f, 6360.f, 5960.f));
	StartingRotations.Add(FRotator(0.f, -50.f, 0.f));

	// Index 4 - Level 5
	Levels.Add(1);
	StartingLocations.Add(FVector(19370.f, 9710.f, 6120.f));
	StartingRotations.Add(FRotator(0.f, -50.f, 0.f));

	// Index 5 - Level 6
	Levels.Add(1);
	StartingLocations.Add(FVector(18160.f, 6360.f, 5960.f));
	StartingRotations.Add(FRotator(0.f, -50.f, 0.f));
}

void ULevelComponent::LevelUpdate(int32 LevelOfBox)
{
	UE_LOG(LogTemp, Warning, TEXT("LevelOfBox: %i, Current Level: %i, Current Progress: %i, # Boxes this level: %i"), LevelOfBox, CurrentLevel, Progress, Levels[CurrentLevel - 1]);
	if (LevelOfBox != CurrentLevel) return;
	
	Progress++;
	if (Progress == Levels[CurrentLevel - 1])
	{
		Owner->LevelComplete(CurrentLevel + 1, StartingLocations[CurrentLevel], StartingRotations[CurrentLevel]);
		Progress = 0;
		CurrentLevel++;
		UE_LOG(LogTemp, Warning, TEXT("ULevelComponent::LevelUpdate(): Leveled up to %i"), CurrentLevel);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ULevelComponent::LevelUpdate(): Did not level up"));
	}
}

void ULevelComponent::Setup(AXtionsCharacter* Char)
{
	Owner = Char;
}

void ULevelComponent::BeginPlay()
{
	Super::BeginPlay();

	
}
