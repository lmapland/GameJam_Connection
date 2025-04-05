// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UOverlay;
class UTextBlock;
class UHorizontalBox;
class UOverlayWidgetController;
class UWidgetAnimation;
class UMissionItemsContainer;

/**
 *
 */
UCLASS()
class CONNECTION_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetController(UOverlayWidgetController* InWidgetController);

	UFUNCTION()
	void DisplayConnectionText();

	UFUNCTION()
	void DisplayLevelCompleteText();

	UFUNCTION()
	void DisplayLevelFailedText();

	UFUNCTION()
	void DisplayTransportingText();

	UFUNCTION()
	void DisplayTutorialText();

	UFUNCTION()
	void UpdateHits();

	//UFUNCTION()
	//void DisplayWinText();

	UFUNCTION()
	void DisplayInteractionText(bool bIsVisible, bool bReadyToRepair);

	UFUNCTION()
	void DisplayDodgesText(int32 Dodges);

	//UFUNCTION()
	//void DisplayJumpsText(int32 Jumps);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHurt();

	UFUNCTION()
	void SetHits();

	UFUNCTION(BlueprintImplementableEvent)
	void HitsOverMaximum();

	UFUNCTION(BlueprintImplementableEvent)
	void HitsUnderMaximum();

	UFUNCTION(BlueprintCallable)
	void EndLevel();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section1;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Section2Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Section3Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* InteractionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* InteractionHorizBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* BoxesText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DodgesText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HitsText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HoverName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* IntraMissionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMissionItemsContainer* MissionItemsContainer;


	int32 Hits = 0;

	int32 CurrentDodges = -1;

	UPROPERTY(BlueprintReadOnly)
	bool GameOver = false;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetKeyBindIcons();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RebuildCustomInputMappings();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayMissionFailedScreen(UOverlayWidget* InOverlayWidget);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayRepairNotReadyText();

private:
	void HideSection1Text();
	void HideSection2Text();
	void HideSection3Text();

	UFUNCTION()
	void DisplayNewLevelInfo(int32 InTotalConnectionBoxes, int32 InMaxHits, TArray<int32> InRequiredObjects, TArray<int32> InObjectCounts);

	UFUNCTION()
	void RebuildKeyBindings();

	UFUNCTION()
	void DisplayHoverText(FString HoverText);

	UFUNCTION()
	void UpdateInteractableInfo(int32 InID, int32 CurrentCount, int32 TotalCount, bool InShowEnabled);

	UFUNCTION()
	void LaunchMissionFailedScreen();

	UFUNCTION()
	void UpdateIntraMissionText(int32 InMissionState, bool bInNewLevel);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UOverlayWidgetController* WidgetController;

	FTimerHandle Section1Handle;
	FTimerHandle Section2Handle;
	FTimerHandle Section3Handle;
	FTimerHandle Section4Handle;

	float TutorialTime = 6.f;
	float TransportingTime = 6.f;
	float WinTime = 5.f;
	float LevelCompleteTime = 3.f;
	float ConnectionTime = 3.f;
	float PlayerErrorTime = 2.f;

	int32 CurrentBox = 0;
	int32 TotalBoxes = 1;

	int32 MaxHits = 0;
	bool bNoUpdateMissionText = false;
};
