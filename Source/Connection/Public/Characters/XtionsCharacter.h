// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "XtionsCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesUpdatedSignature, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlappingBox, bool, bIsOverlapping);

class AConnectionBox;
class UOverlayWidget;
class USoundBase;

UCLASS()
class CONNECTION_API AXtionsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AXtionsCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Damage(int32 Amount);
	void TransportCharacter(FVector Location, FRotator Rotation);
	void SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox);

	/* Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
	int32 Health = 4; // 4 is easy, 2 is medium, 1 is hard

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
	int32 MaxHealth = 4;

	UPROPERTY()
	FOnLivesUpdatedSignature OnLivesUpdated;

	UPROPERTY()
	FOnCharacterDeathSignature OnCharacterDeath;

	UPROPERTY()
	FOnOverlappingBox OnOverlappingBox;
	
	UPROPERTY(EditAnywhere, Category = "Character|Attributes")
	USoundBase* OnDamageSound;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	class UInputMappingContext* CharMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	class UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Input")
	class UInputAction* InteractAction;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& value);
	void Die();
	void PostDie();
	void PlaySound(USoundBase* SoundToPlay);

	APlayerController* PlayerController;

	AConnectionBox* OverlappedBox;

	bool bAlive = true;

	FVector TransportLoc;
	FRotator TransportRot;

	FTimerHandle DeathTimer;
	float DeathTimout = 5.f;

public:
	FORCEINLINE bool IsAlive() const { return bAlive; }
	FORCEINLINE int32 GetStartLives() const { return Health; }
};
