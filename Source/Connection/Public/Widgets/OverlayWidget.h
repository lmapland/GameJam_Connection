// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UProgressBar;
class UOverlayWidgetController;

/**
 *
 */
UCLASS()
class CONNECTION_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetController(UOverlayWidgetController* InWidgetController);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayConnectionText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayLevelCompleteText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayTransportingText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayTutorialText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayDeathText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayWinText();


	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHurt();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerGainedHealth();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLives(int32 DefaultLives);

	/*
	 * This function is called after error-checking takes place.
	 * Here we assume the XtionsCharacter knows the character can have lives
	 *  added or subtracted and only calls this function when health > 0 and health < maxhealth
	 */
	UFUNCTION()
	void UpdateLives(int32 Amount);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UOverlayWidgetController* WidgetController;

};
