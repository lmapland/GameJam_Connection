// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowTutorialSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitializeHitsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeDodgesSignature, int32, Dodges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateDodgesSignature, int32, Dodges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateJumpsSignature, int32, Jumps);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncrementHitsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowPlayerWinTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowCharacterTransportTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowLevelCompleteTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowConnectionMadeTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowInteractionText, bool, isOverlapping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTotalLevels, int32, TotalLevels);

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
	FOnIncrementHitsSignature IncrementHits;

	UPROPERTY()
	FOnInitializeHitsSignature OnInitializeHits;

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

	UPROPERTY()
	FOnUpdateJumpsSignature OnUpdateJumpsText;

	UPROPERTY()
	FOnUpdateTotalLevels OnUpdateTotalLevels;

private:
	UFUNCTION()
	void DisplayTutorialText();

	UFUNCTION()
	void DisplayHitsText();

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

	UFUNCTION()
	void DisplayJumpsText(int32 Amount);

	UFUNCTION()
	void UpdateTotalLevels(int32 TotalLevels);

	UPROPERTY()
	ULevelManager* LevelManager;

	UPROPERTY()
	AXtionsCharacter* Character;

};
