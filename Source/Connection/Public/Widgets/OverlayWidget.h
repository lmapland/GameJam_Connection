// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UProgressBar;

/**
 *
 */
UCLASS()
class CONNECTION_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayConnectionText();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayLevelCompleteText();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayTransportingText();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayTutorialText();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayDeathText();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayWinText();


	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHurt();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerGainedHealth();
	
	/* The default is "easy" or 4 lives. Hard is 1, Medium is 2 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetLives(int32 DefaultLives);

	/*
	 * This function is called after error-checking takes place.
	 * Here we assume the XtionsCharacter knows the character can have lives
	 *  added or subtracted and only calls this function when health > 0 and health < maxhealth
	 */
	UFUNCTION()
	void UpdateLives(int32 Amount);

};
