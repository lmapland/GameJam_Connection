// Copyright Leslie Apland


#include "UI/LevelMissionDescription.h"
#include "UI/LevelSelection.h"
#include "Components/TextBlock.h"

void ULevelMissionDescription::Setup(ULevelSelection* InLevelSelection)
{
	LevelSelection = InLevelSelection;
	SetFocus();

	LevelTexts.Add(Level1Text);
	LevelTexts.Add(Level2Text);
	LevelTexts.Add(Level3Text);
	LevelTexts.Add(Level4Text);
}

void ULevelMissionDescription::ShowLevelText(int32 InLevel)
{
	HideAllText();
	LevelTexts[InLevel - 1]->SetVisibility(ESlateVisibility::Visible);
}

void ULevelMissionDescription::Continue()
{
	SetVisibility(ESlateVisibility::Hidden);
	LevelSelection->LoadLevel();
}

void ULevelMissionDescription::HideAllText()
{
	for (UTextBlock* TextBlock : LevelTexts)
	{
		TextBlock->SetVisibility(ESlateVisibility::Hidden);
	}
}
