// Copyright Leslie Apland


#include "UI/MissionItemsContainer.h"
#include "Components/VerticalBox.h"

void UMissionItemsContainer::Setup(const TArray<int32>& ItemIDs, const TArray<int32>& ItemCounts)
{
	ItemsBox->ClearChildren();

	for (int i = 0; i < ItemIDs.Num(); i++)
	{
		AddChild(ItemIDs[i], 0);
	}
}

void UMissionItemsContainer::UpdateItem(int32 ItemID, int32 Count, bool ShowEnabled)
{
	UpdateChild(ItemID, Count, ShowEnabled);
}
