// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowTutorialSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeLivesSignature, int32, Lives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateLivesSignature, int32, Lives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeDodgesSignature, int32, Dodges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateDodgesSignature, int32, Dodges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowCharacterDeathTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowPlayerWinTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowCharacterTransportTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowLevelCompleteTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowConnectionMadeTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowInteractionText, bool, isOverlapping);

class AXtionsCharacter;
class ULevelManager;
/**
 * 
 */
UCLASS()
class CONNECTION_API UOverlayWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	void SetWidgetControllerParams(AXtionsCharacter* InCharacter, ULevelManager* InLevelManager);
	void BindCallbacksToDependencies();
	void BroadcastInitialValues();
	
	UPROPERTY()
	FOnShowTutorialSignature OnShowTutorial;

	UPROPERTY()
	FOnInitializeLivesSignature OnInitializeLives;

	UPROPERTY()
	FOnUpdateLivesSignature OnUpdateLives;

	UPROPERTY()
	FOnShowCharacterDeathTextSignature OnShowCharacterDeathText;

	UPROPERTY()
	FOnShowPlayerWinTextSignature OnShowPlayerWinText;

	UPROPERTY()
	FOnShowCharacterTransportTextSignature OnShowCharacterTransportText;

	UPROPERTY()
	FOnShowLevelCompleteTextSignature OnShowLevelCompleteText;

	UPROPERTY()
	FOnShowConnectionMadeTextSignature OnShowConnectionMadeText;

	UPROPERTY()
	FOnShowInteractionText OnShowInteractionText;

	UPROPERTY()
	FOnInitializeDodgesSignature OnInitializeDodges;
	
	UPROPERTY()
	FOnUpdateDodgesSignature OnUpdateDodgesText;

private:
	UFUNCTION()
	void DisplayTutorialText();

	UFUNCTION()
	void UpdateHUDLives(int32 Amount);

	UFUNCTION()
	void DisplayDeathText();

	UFUNCTION()
	void DisplayWinText();

	UFUNCTION()
	void DisplayTransportText();

	UFUNCTION()
	void DisplayConnectionText();

	UFUNCTION()
	void DisplayLevelCompleteText();

	UFUNCTION()
	void DisplayInterationText(bool bOverlapping);

	UFUNCTION()
	void DisplayDodgesText(int32 Amount);

	UPROPERTY()
	ULevelManager* LevelManager;

	UPROPERTY()
	AXtionsCharacter* Character;


};
