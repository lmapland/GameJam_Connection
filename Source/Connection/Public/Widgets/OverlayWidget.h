// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

class UOverlay;
class UTextBlock;
class UOverlayWidgetController;
class UWidgetAnimation;

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
	void DisplayTransportingText();

	UFUNCTION()
	void DisplayTutorialText();

	UFUNCTION()
	void DisplayDeathText();

	UFUNCTION()
	void DisplayWinText();

	UFUNCTION()
	void DisplayInteractionText(bool bIsVisible);

	UFUNCTION()
	void DisplayDodgesText(int32 Dodges);

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section1;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section3;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Section4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Section2Text;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Section4Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* InteractionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DodgesText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DodgesAnimText;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* NewDodgeAnimation;

	int32 CurrentLives = 0;

	int32 CurrentDodges = -1;

	UPROPERTY(BlueprintReadOnly)
	bool GameOver = false;

protected:
	virtual void NativeConstruct() override;

private:
	void HideSection1Text();
	void HideSection2Text();
	void HideSection3Text();
	void HideSection4Text();
	void DisplayTutorialPart2Text();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UOverlayWidgetController* WidgetController;

	FTimerHandle Section1Handle;
	FTimerHandle Section2Handle;
	FTimerHandle Section3Handle;
	FTimerHandle Section4Handle;

	float TutorialTime = 6.f;
	float TransportingTime = 6.f;
	float DeathTime = 5.f;
	float WinTime = 5.f;
	float LevelCompleteTime = 3.f;
	float ConnectionTime = 3.f;
};
