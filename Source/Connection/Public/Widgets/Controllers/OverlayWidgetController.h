// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowTutorialSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitializeHitsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeDodgesSignature, int32, Dodges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateDodgesSignature, int32, Dodges);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIncrementHitsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowCharacterTransportTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowLevelCompleteTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowLevelFailedTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowConnectionMadeTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShowInteractionText, bool, isOverlapping, bool, bReadyToRepair);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnShowNewLevelInfo, int32, InTotalConnectionBoxes, int32, InMaxHits, TArray<int32>, InRequiredObjects, TArray<int32>, InObjectCounts);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRebuildKeyBindings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHoverText, FString, HoverText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateInteractableInfo, int32, InID, int32, CurrentCount, int32, TotalCount, bool, InshowEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisplayRepairNotReadyText);

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
	FOnShowCharacterTransportTextSignature OnShowCharacterTransportText;

	UPROPERTY()
	FOnShowLevelCompleteTextSignature OnShowLevelCompleteText;

	UPROPERTY()
	FOnShowLevelFailedTextSignature OnShowLevelFailedText;

	UPROPERTY()
	FOnShowConnectionMadeTextSignature OnShowConnectionMadeText;

	UPROPERTY()
	FOnShowInteractionText OnShowInteractionText;

	UPROPERTY()
	FOnInitializeDodgesSignature OnInitializeDodges;

	UPROPERTY()
	FOnUpdateDodgesSignature OnUpdateDodgesText;

	UPROPERTY()
	FOnShowNewLevelInfo OnShowNewLevelInfo;

	UPROPERTY()
	FOnRebuildKeyBindings OnRebuildKeyBindings;

	UPROPERTY()
	FOnUpdateHoverText OnUpdateHoverText;

	UPROPERTY()
	FOnUpdateInteractableInfo OnUpdateInteractableInfo;

	UPROPERTY()
	FOnDisplayRepairNotReadyText OnDisplayRepairNotReadyText;

private:
	UFUNCTION()
	void DisplayTutorialText();

	UFUNCTION()
	void DisplayHitsText();

	UFUNCTION()
	void DisplayTransportText();

	UFUNCTION()
	void DisplayConnectionText();

	UFUNCTION()
	void DisplayLevelCompleteText(bool bLevelSuccessfullyCompleted);

	UFUNCTION()
	void DisplayInterationText(bool bOverlapping, bool bReadyToRepair);

	UFUNCTION()
	void DisplayDodgesText(int32 Amount);

	UFUNCTION()
	void UpdateNewLevelInfo(int32 InTotalConnectionBoxes, int32 InMaxHits, TArray<int32> InRequiredObjects, TArray<int32> InObjectCounts);

	UFUNCTION()
	void SendRebuildKeyBindings();

	UFUNCTION()
	void DisplayHoverText(FString HoverText);

	UFUNCTION()
	void UpdateInteractableInfo(int32 InID, int32 CurrentCount, int32 TotalCount, bool InShowEnabled);

	UFUNCTION()
	void DisplayRepairNotReadyText();

	UPROPERTY()
	ULevelManager* LevelManager;

	UPROPERTY()
	AXtionsCharacter* Character;

};
