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
	Levels.Add(FLevelInfo());
	Levels[0].MaxHits = 3;
	Levels[0].Location = FVector(41231.f, -64797.f, 4390.f);
	Levels[0].Rotation = FRotator(0.f, 30.f, 0.f);
	Levels[0].RequiredObjects.Add(1);
	Levels[0].ObjectCounts.Add(1);

	// Index 1 - Level 2
	Levels.Add(FLevelInfo());
	Levels[1].Connections = 2;
	Levels[1].Location = FVector(25952.f, -45933.f, 3100.f);
	Levels[1].Rotation = FRotator(0.f, 0.f, 0.f);
	Levels[1].RequiredObjects.Add(2);
	Levels[1].ObjectCounts.Add(3);

	// Index 2 - Level 3
	Levels.Add(FLevelInfo());
	Levels[2].Connections = 1;
	Levels[2].Location = FVector(26635.f, -15786.f, 4194.f);
	//Levels[2].Location = FVector(29357.f, -18224.f, 5135.f);
	Levels[2].Rotation = FRotator(0.f, -117.f, 0.f);
	Levels[2].RequiredObjects.Add(4);
	Levels[2].RequiredObjects.Add(3);
	Levels[2].ObjectCounts.Add(1);
	Levels[2].ObjectCounts.Add(3);

	// Index 3 - Level 4
	//Levels.Add(FLevelInfo());
	//Levels[3].Connections = 2;
	//Levels[3].Location = FVector(18196.f, 6319.f, 5960.f);
	//Levels[3].Rotation = FRotator(0.f, 90.f, 0.f);

	/*
	// Index 4 - Level 5
	Levels.Add(1);
	StartingLocations.Add(FVector(19370.f, 9710.f, 6120.f));
	StartingRotations.Add(FRotator(0.f, 0.f, 0.f));

	// Index 5 - Level 6
	Levels.Add(1);
	StartingLocations.Add(FVector(18160.f, 6360.f, 5960.f));
	StartingRotations.Add(FRotator(0.f, 0.f, 0.f));
	*/
}

void ULevelManager::Setup(TArray<int32> InLevelCompletions, TArray<float> InLevelTimes)
{
	LevelCompletions = InLevelCompletions;
	LevelTimes = InLevelTimes;

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
	Player->OnLevelSelectOpened.AddUniqueDynamic(this, &ULevelManager::TearDownLevel);
	Player->OnCharacterHit.AddUniqueDynamic(this, &ULevelManager::PlayerHit);

	// Load default LevelTimes, LevelSelections values
	for (int i = 0; i < Levels.Num(); i++)
	{
		if (LevelCompletions.Num() <= i)
		{
			LevelCompletions.Add(0);
		}
		if (LevelTimes.Num() <= i)
		{
			LevelTimes.Add(MAX_FLT);
		}
		OnLevelStatsUpdated.Broadcast(i + 1, LevelCompletions[i], LevelTimes[i]);
	}
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
	LastLevelPlayed = CurrentLevel;
	CurrentHits = 0;

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
	InitialTime = GetWorld()->GetTimeSeconds();
}

void ULevelManager::ConnectionComplete(int32 LevelOfBox)
{
	//UE_LOG(LogTemp, Warning, TEXT("ConnectionComplete: %i, Current Level: %i, Current Progress: %i, # Boxes this level: %i"), LevelOfBox, CurrentLevel, Progress, Levels[CurrentLevel - 1].Connections);
	if (LevelOfBox != CurrentLevel) return;

	OnConnectionMade.Broadcast();
	Progress++;

	// Is the level complete?
	if (Progress == Levels[CurrentLevel - 1].Connections)
	{
		// Should the player progress to the next level or replay this one?
		bool bLevelSuccessfullyCompleted = Player->GetHits() <= Levels[CurrentLevel - 1].MaxHits ? true : false;
		OnLevelComplete.Broadcast(bLevelSuccessfullyCompleted);
		
		Progress = 0;
		bCurrentLevelStarted = false;

		if (bLevelSuccessfullyCompleted)
		{
			// TODO time the level and update that time here
			LevelCompletions[CurrentLevel - 1] += 1;
			LevelTime = GetWorld()->GetTimeSeconds() - InitialTime;
			if (LevelTime < LevelTimes[CurrentLevel - 1])
			{
				LevelTimes[CurrentLevel - 1] = LevelTime;
			}
			//UE_LOG(LogTemp, Warning, TEXT("ConnectionComplete(): %f - %f = %f seconds"), GetWorld()->GetTimeSeconds(), InitialTime, LevelTime);
			OnLevelStatsUpdated.Broadcast(CurrentLevel, LevelCompletions[CurrentLevel - 1], LevelTimes[CurrentLevel - 1]);

			CurrentLevel++;
			if (CurrentLevel == MaxLevel)
			{
				GetWorld()->GetTimerManager().SetTimer(TransportTimer, this, &ULevelManager::EndTheGame, TransportTime);
			}
			else
			{
				OnCharacterTransport.Broadcast();
				GetWorld()->GetTimerManager().SetTimer(TransportTimer, this, &ULevelManager::TransportPlayer, TransportTime);
			}
		}
		else
		{
			OnCharacterTransport.Broadcast();
			GetWorld()->GetTimerManager().SetTimer(TransportTimer, this, &ULevelManager::RestartLevel, TransportTime);
			// Figure out how to bind params to a Timer and call a different function instead
			// That different function needs to call TearDownLevel and then StartLevel(CurrentLevel) instead
			// Since the player is restarting the current level. Yuck.
		}
	}
}

void ULevelManager::EndTheGame()
{
	Player->OpenLevelSelect();
}

void ULevelManager::RestartLevel()
{
	TearDownLevel();
	StartLevel(CurrentLevel);
}

void ULevelManager::TearDownLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("ULevelManager::TearDownLevel(): %i"), LastLevelPlayed);
	TArray<AActor*> AllActors;
	FString LevelString("");
	LevelString.AppendInt(LastLevelPlayed);
	UGameplayStatics::GetAllActorsWithTag(this, FName(*LevelString), AllActors);

	for (auto CurrentActor : AllActors)
	{
		if (auto ActorCanTurnOn = Cast<ICanTurnOn>(CurrentActor))
		{
			ActorCanTurnOn->Stop();
		}
	}
	Progress = 0;
}

void ULevelManager::PlayerHit()
{
	CurrentHits++;
	//UE_LOG(LogTemp, Warning, TEXT("ULevelManager::PlayerHit(): CurrentHits: %i"), CurrentHits);

	/* End the level if the player has been hit too many times */
	if (CurrentHits > Levels[CurrentLevel - 1].MaxHits)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ULevelManager::PlayerHit(): Player has been hit too many times"));
		// broadcast that the player should be pulled from the level
		OnPlayerHitTooManyTimes.Broadcast();
		Player->Die();
	}
}

void ULevelManager::StartLevel(int32 LevelToStart)
{
	//UE_LOG(LogTemp, Warning, TEXT("ULevelManager::StartLevel(): Getting all actors with tag (level) %i"), LevelToStart);
	if (LevelToStart > Levels.Num() || LevelToStart < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid level passed in. Level %i is not in array Levels.Num() (%i)"), LevelToStart, Levels.Num());
		return;
	}

	CurrentLevel = LevelToStart;
	//OnNewLevel.Broadcast(Levels[CurrentLevel - 1].Connections, Levels[CurrentLevel - 1].MaxHits);
	OnStartLevel.Broadcast(CurrentLevel);

	if (CurrentLevel == 1)
	{
		OnLaunchTutorial.Broadcast();
	}

	TArray<AActor*> AllActors;
	FString LevelString("");
	LevelString.AppendInt(CurrentLevel);
	UGameplayStatics::GetAllActorsWithTag(this, FName(*LevelString), AllActors);

	for (auto CurrentActor : AllActors)
	{
		if (auto ActorCanTurnOn = Cast<ICanTurnOn>(CurrentActor))
		{
			ActorCanTurnOn->Prepare();
		}
	}

	TransportPlayer(LevelToStart);
}

void ULevelManager::TransportPlayer()
{
	TearDownLevel();
	StartLevel(CurrentLevel);
	//TransportPlayer(CurrentLevel);
}

void ULevelManager::TransportPlayer(int32 ToLevel)
{
	OnNewLevel.Broadcast(Levels[ToLevel - 1].Connections, Levels[ToLevel - 1].MaxHits, Levels[ToLevel - 1].RequiredObjects, Levels[ToLevel - 1].ObjectCounts);
	Player->TransportCharacter(Levels[ToLevel - 1]);
	bCurrentLevelStarted = false;
}

void ULevelManager::EndLevelPrematurely()
{
	OnCharacterTransport.Broadcast();
	RestartLevel();
}
