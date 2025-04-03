// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "XtionsCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDodgesUpdatedSignature, int32, Total);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashesUpdated, int32, Total);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterHitSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlappingBox, bool, bIsOverlapping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelSkipRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelSelectOpenedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyBindingsNeedRebuilt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredInteractable, FString, InteractableName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPickedUpInteractable, int32, ID, int32, TotalCount, bool, bHighlight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotReadyToRepair);

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class UInputComponent;
class AConnectionBox;
class UOverlayWidget;
class USoundBase;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class UXtionsGameInstance;
struct FLevelInfo;

UCLASS()
class CONNECTION_API AXtionsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AXtionsCharacter();
	void Tick(float DeltaTime);
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Damage(int32 Amount);
	void TransportCharacter(FLevelInfo& LevelInfo);
	void SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox);
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	void OpenLevelSelect();

	UFUNCTION()
	virtual void OnItemSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnItemSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
	int32 NumDodges = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
	int32 NumDashes = 5;

	UPROPERTY()
	FOnCharacterHitSignature OnCharacterHit;

	UPROPERTY()
	FOnLevelSkipRequested OnLevelSkipRequested;

	UPROPERTY()
	FOnOverlappingBox OnOverlappingBox;

	UPROPERTY()
	FOnDodgesUpdatedSignature OnDodgesUpdated;

	UPROPERTY()
	FOnDashesUpdated OnDashesUpdated;

	UPROPERTY()
	FOnLevelSelectOpenedSignature OnLevelSelectOpened;

	UPROPERTY(BlueprintCallable)
	FOnKeyBindingsNeedRebuilt OnKeyBindingsNeedRebuilt;

	UPROPERTY()
	FOnHoveredInteractable OnHoveredInteractable;

	UPROPERTY()
	FOnPickedUpInteractable OnPickedUpInteractable;

	UPROPERTY()
	FOnNotReadyToRepair OnNotReadyToRepair;
	
	UPROPERTY(EditAnywhere, Category = "Character|Attributes")
	USoundBase* OnDamageSound;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	float GetXSensitivity();

	UFUNCTION(BlueprintImplementableEvent)
	float GetYSensitivity();

	UFUNCTION(BlueprintImplementableEvent)
	bool InvertXAxis();

	UFUNCTION(BlueprintImplementableEvent)
	bool InvertYAxis();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputMappingContext* CharMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* LeaveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* LevelSkipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* LevelSelectAction;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Montages")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Montages")
	UAnimMontage* DashMontage;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& value);
	void Dodge(const FInputActionValue& value);
	void Dash();
	void LeaveGame(const FInputActionValue& value); /* Technically, quit the current level back to the level selection screen */
	void SkipCurrentLevel();
	void LineTraceForInteractable();
	bool PerformLineTrace(FHitResult& HitResult); //ETraceTypeQuery TraceTypeQuery = ETraceTypeQuery::TraceTypeQuery4
	bool ReadyToRepair();

	UFUNCTION(BlueprintCallable)
	void DodgeFinish();

	UFUNCTION(BlueprintCallable)
	void DashFinish();

	void PlaySound(USoundBase* SoundToPlay);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* ItemSphere;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UXtionsGameInstance* GameInstance;

	UPROPERTY()
	AConnectionBox* OverlappedBox;

	FVector TransportLoc;
	FRotator TransportRot;

	bool bIsDodging = false;
	bool bIsJumping = false;
	bool bIsDashing = false;

	int32 Hits = 0;

	TArray<int32> RequiredObjects;
	TArray<int32> ObjectCounts;
	TArray<int32> CurrentObjectCounts;

	float LineTraceDistance = 500.f;
	AActor* HoveredInteractable = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> OverlappingActors;

public:
	FORCEINLINE int32 GetStartDodges() const { return NumDodges; }
	FORCEINLINE int32 GetStartDashes() const { return NumDashes; }
	FORCEINLINE int32 GetHits() const { return Hits; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharacterMovementComponent* GetXCharacterMovement() const { return GetCharacterMovement(); }
};
