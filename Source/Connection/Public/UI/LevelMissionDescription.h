// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelMissionDescription.generated.h"

class ULevelSelection;
class UTextBlock;

/**
 * 
 */
UCLASS()
class CONNECTION_API ULevelMissionDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(ULevelSelection* InLevelSelection);
	void ShowLevelText(int32 InLevel);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level1Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level2Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Level3Text;

protected:
	UFUNCTION(BlueprintCallable)
	void Continue();

private:
	void HideAllText();

	UPROPERTY()
	ULevelSelection* LevelSelection;

	TArray<UTextBlock*> LevelTexts;
};
