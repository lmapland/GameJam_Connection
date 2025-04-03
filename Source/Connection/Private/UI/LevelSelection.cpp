// Copyright Leslie Apland


#include "UI/LevelSelection.h"
#include "Components/TextBlock.h"
#include "LevelManagement/LevelManager.h"
#include "UI/LevelMissionDescription.h"

void ULevelSelection::Setup(ULevelManager* InLevelManager)
{
	InLevelManager->OnLevelStatsUpdated.AddDynamic(this, &ULevelSelection::LevelStatsUpdated);
	MissionDescription->Setup(this);

	CompletionTimeTextBlocks.Add(Level1CompletionTime);
	CompletionTimeTextBlocks.Add(Level2CompletionTime);
	CompletionTimeTextBlocks.Add(Level3CompletionTime);

	NumCompletionTextBlocks.Add(Level1NumCompletions);
	NumCompletionTextBlocks.Add(Level2NumCompletions);
	NumCompletionTextBlocks.Add(Level3NumCompletions);
}

void ULevelSelection::ShowMissionDescription(int32 InLevel)
{
	MissionDescription->ShowLevelText(InLevel);
	MissionDescription->SetVisibility(ESlateVisibility::Visible);
	MissionDescription->SetFocus();
}

void ULevelSelection::LevelStatsUpdated(int32 InLevel, int32 NumCompletions, float CompletionTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("ULevelSelection::LevelStatsUpdated(): %i, %f"), NumCompletions, CompletionTime);
	int32 CurrentLevelIndex = InLevel - 1;

	if (CurrentLevelIndex >= CompletionTimeTextBlocks.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ULevelSelection::LevelStatsUpdated(): CurrentLevelIndex out of bounds for CompletionTimeTextBlocks: %i >= %i"), CurrentLevelIndex >= CompletionTimeTextBlocks.Num());
		return;
	}
	if (CurrentLevelIndex >= NumCompletionTextBlocks.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ULevelSelection::LevelStatsUpdated(): CurrentLevelIndex out of bounds for NumCompletionTextBlocks: %i >= %i"), CurrentLevelIndex >= NumCompletionTextBlocks.Num());
		return;
	}

	FString TimeString = FString::Printf(TEXT("%f"), CompletionTime);
	if (FMath::IsNearlyEqual(CompletionTime, MAX_FLT))
	{
		CompletionTimeTextBlocks[CurrentLevelIndex]->SetText(FText::FromString(FString("--")));
	}
	else
	{
		float TotalTime = CompletionTime;
		int32 Hours = int(TotalTime / 3600);
		TotalTime -= Hours * 3600;
		int32 Minutes = int(TotalTime / 60);
		TotalTime -= Minutes * 60;
		int32 Seconds = int(TotalTime); // FMath::RoundHalfToZero(100.f * TotalTime);
		TotalTime -= Seconds;
		int32 Miliseconds = int(TotalTime * 100.f);
		//FString SecondsString = FString::SanitizeFloat(Seconds, 2);
		CompletionTimeTextBlocks[CurrentLevelIndex]->SetText(FText::FromString(FString::Printf(TEXT("%i:%i:%i.%i"), Hours, Minutes, Seconds, Miliseconds)));
	}
	NumCompletionTextBlocks[CurrentLevelIndex]->SetText(FText::FromString(FString::Printf(TEXT("%i"), NumCompletions)));
}
