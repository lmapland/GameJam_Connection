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
	void DisplayConnectionText();
	void HideConnectionText();
	void DisplayLevelCompleteText();
	void HideLevelCompleteText();
	void DisplayTransportingText();
	void HideTransportingText();
	void DisplayTutorialText();
	void HideTutorialText();
	void DisplayDeathText();
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ConnectionText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* LevelCompleteText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TransportingText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TutorialText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DeathText;

private:
	FTimerHandle ConnectTimer;
	FTimerHandle LevelTimer;
	FTimerHandle TransportTimer;
	FTimerHandle TutorialTimer;

	float TextClearedTime = 5.f;
	float TransportTime = 7.f;

};
