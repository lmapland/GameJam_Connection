// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "XtionsCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDodgesUpdatedSignature, int32, Total);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpsUpdatedSignature, int32, Total);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterHitSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlappingBox, bool, bIsOverlapping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelSkipRequested);

class UInputComponent;
class AConnectionBox;
class UOverlayWidget;
class USoundBase;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;

UCLASS()
class CONNECTION_API AXtionsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AXtionsCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Damage(int32 Amount);
	void TransportCharacter(FVector Location, FRotator Rotation);
	void SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox);
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
	int32 NumDodges = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
	int32 NumJumps = 10;

	UPROPERTY()
	FOnCharacterHitSignature OnCharacterHit;

	UPROPERTY()
	FOnLevelSkipRequested OnLevelSkipRequested;

	UPROPERTY()
	FOnOverlappingBox OnOverlappingBox;

	UPROPERTY()
	FOnDodgesUpdatedSignature OnDodgesUpdated;

	UPROPERTY()
	FOnJumpsUpdatedSignature OnJumpsUpdated;
	
	UPROPERTY(EditAnywhere, Category = "Character|Attributes")
	USoundBase* OnDamageSound;

protected:
	virtual void BeginPlay() override;
	
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
	UInputAction* LeaveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	UInputAction* LevelSkipAction;

	UPROPERTY(EditDefaultsOnly, Category = "Character|Montages")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	float LookSensitivity = 1.f;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& value);
	void Dodge(const FInputActionValue& value);
	void LeaveGame(const FInputActionValue& value);
	void SkipCurrentLevel();

	UFUNCTION(BlueprintCallable)
	void DodgeFinish();

	void PlaySound(USoundBase* SoundToPlay);

	APlayerController* PlayerController;

	AConnectionBox* OverlappedBox;

	FVector TransportLoc;
	FRotator TransportRot;

	bool bIsDodging = false;
	bool bIsJumping = false;

public:
	FORCEINLINE int32 GetStartDodges() const { return NumDodges; }
	FORCEINLINE int32 GetStartJumps() const { return NumJumps; }
};
