// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionItemsContainer.generated.h"

class UVerticalBox;

/**
 * 
 */
UCLASS()
class CONNECTION_API UMissionItemsContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(const TArray<int32>& ItemIDs, const TArray<int32>& ItemCounts);

	void UpdateItem(int32 ItemID, int32 CurrentCount, int32 TotalCount, bool ShowEnabled);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* ItemsBox;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void AddChild(int32 ItemID, int32 CurrentCount, int32 TotalCount);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateChild(int32 ItemID, int32 CurrentCount, int32 TotalCount, bool ShowEnabled);

};
