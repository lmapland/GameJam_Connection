// Copyright Leslie Apland


#include "LevelManagement/LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "LevelManagement/StartZone.h"
#include "Enemies/Enemy.h"
#include "Connections/ConnectionBox.h"
#include "Characters/XtionsCharacter.h"

ULevelManager::ULevelManager()
{
	// Index 0 - Level 1
	Levels.Add(1); // To finish level 1 (0th slot), only 1 ConnectionBox needs to be connected
	StartingLocations.Add(FVector(0.f)); // Index 0 should be empty as the character starts, for level 1, at the location of the Player Start
	StartingRotations.Add(FRotator(0.f));

	// Index 1 - Level 2
	Levels.Add(2);
	StartingLocations.Add(FVector(14600.f, -3160.f, 5598.f));
	//StartingLocations.Add(FVector(19694.f, -451.f, 5946.f));
	StartingRotations.Add(FRotator(0.f, 90.f, 0.f));

	// Index 2 - Level 3
	Levels.Add(2);
	StartingLocations.Add(FVector(12320.f, -7170.f, 5946.f));
	StartingRotations.Add(FRotator(0.f, 0.f, 0.f));

	// Index 3 - Level 4
	Levels.Add(2);
	StartingLocations.Add(FVector(18196.f, 6319.f, 5960.f));
	StartingRotations.Add(FRotator(0.f, -90.f, 0.f));

	// Index 4 - Level 5
	Levels.Add(1);
	StartingLocations.Add(FVector(19370.f, 9710.f, 6120.f));
	StartingRotations.Add(FRotator(0.f, 0.f, 0.f));

	// Index 5 - Level 6
	Levels.Add(1);
	StartingLocations.Add(FVector(18160.f, 6360.f, 5960.f));
	StartingRotations.Add(FRotator(0.f, 0.f, 0.f));

}

void ULevelManager::Setup()
{
	// Bind to Start Zones for all levels
	TArray<AActor*> AllStartZones;
	UGameplayStatics::GetAllActorsOfClass(this, AStartZone::StaticClass(), AllStartZones);

	for (auto CurrentActor : AllStartZones)
	{
		if (AStartZone* Zone = Cast<AStartZone>(CurrentActor))
		{
			Zone->OnCharacterExitDelegate.AddUniqueDynamic(this, &ULevelManager::CharacterInPlay);
		}
	}

	// Bind to Connection Boxes for all levels
	TArray<AActor*> AllConnectionBoxActors;

	UGameplayStatics::GetAllActorsOfClass(this, AConnectionBox::StaticClass(), AllConnectionBoxActors);

	for (auto CurrentActor : AllConnectionBoxActors)
	{
		if (AConnectionBox* CBox = Cast<AConnectionBox>(CurrentActor))
		{
			CBox->OnConnectDelegate.AddUniqueDynamic(this, &ULevelManager::ConnectionComplete);
			AllConnectionBoxes.Add(CBox);
		}
	}

	// Set player character
	AActor* ActorOwner = UGameplayStatics::GetActorOfClass(this, AXtionsCharacter::StaticClass());
	if (ActorOwner) Player = Cast<AXtionsCharacter>(ActorOwner);
	Player->OnLevelSkipRequested.AddUniqueDynamic(this, &ULevelManager::SkipCurrentLevel);

	OnLaunchTutorial.Broadcast();
}

void ULevelManager::SkipCurrentLevel()
{
	//UE_LOG(LogTemp, Warning, TEXT("SkipCurrentLevel(): Skipping Level %i"), CurrentLevel);
	int32 LevelToSkip = CurrentLevel;
	// To "skip" the current level, we connect all of the boxes associated with this level.
	for (AConnectionBox *CBox : AllConnectionBoxes)
	{
		if (CBox->Level == LevelToSkip)
		{
			CBox->Use();
		}
	}
}

/*
* Aka StartLevel(InLevel)
* The character has stepped out of their startzone. We need to activate all Actors tagged as being part of this level to start the game
* Need to be aware this event can be called multiple times and we need to have a safeguard in place
*/
void ULevelManager::CharacterInPlay(int32 InLevel)
{
	//UE_LOG(LogTemp, Warning, TEXT("CharacterInPlay(): CurrentLevel: %i, InLevel: %i"), CurrentLevel, InLevel);
	if (bCurrentLevelStarted || InLevel != CurrentLevel) return;
	bCurrentLevelStarted = true;

	TArray<AActor*> AllActors;
	FString LevelString("");
	LevelString.AppendInt(InLevel);
	UGameplayStatics::GetAllActorsWithTag(this, FName(*LevelString), AllActors);

	for (auto CurrentActor : AllActors)
	{
		if (auto ActorCanTurnOn = Cast<ICanTurnOn>(CurrentActor))
		{
			ActorCanTurnOn->Start();
		}
	}
}

void ULevelManager::ConnectionComplete(int32 LevelOfBox)
{
	//UE_LOG(LogTemp, Warning, TEXT("ConnectionComplete: %i, Current Level: %i, Current Progress: %i, # Boxes this level: %i"), LevelOfBox, CurrentLevel, Progress, Levels[CurrentLevel - 1]);
	if (LevelOfBox != CurrentLevel) return;

	OnConnectionMade.Broadcast();
	Progress++;

	if (Progress == Levels[CurrentLevel - 1])
	{
		OnLevelComplete.Broadcast();
		Progress = 0;
		CurrentLevel++;
		bCurrentLevelStarted = false;

		if (CurrentLevel == MaxLevel)
		{
			OnPlayerWin.Broadcast();
			GetWorld()->GetTimerManager().SetTimer(TransportTimer, this, &ULevelManager::EndTheGame, TransportTime);
		}
		else
		{
			OnCharacterTransport.Broadcast();
			GetWorld()->GetTimerManager().SetTimer(TransportTimer, this, &ULevelManager::TransportPlayer, TransportTime);
		}
	}
}

void ULevelManager::EndTheGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void ULevelManager::TransportPlayer()
{
	OnNewLevelSignature.Broadcast(Levels[CurrentLevel - 1]);
	Player->TransportCharacter(StartingLocations[CurrentLevel - 1], StartingRotations[CurrentLevel - 1]);
	bCurrentLevelStarted = false;
}
