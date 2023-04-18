// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "XtionsCharacter.generated.h"

class AConnectionBox;
class UOverlayWidget;

UCLASS()
class CONNECTION_API AXtionsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AXtionsCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetOverlappedConnectionBox(AConnectionBox* InOverlappedBox);
	void Damage(float Amount);
	void LevelComplete(int32 NewLevel, FVector NewLoc, FRotator NewRot);

	void EndTheGame();

	/* Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxStamina = 100.f;

	UOverlayWidget* Overlay;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* CharMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* InteractAction;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& value);
	void Die();
	void PostDie();
	void TransportCharacter();

	APlayerController* PlayerController;

	AConnectionBox* OverlappedBox;

	bool bAlive = true;

	FTimerHandle TransportTimer;
	FVector TransportLoc;
	FRotator TransportRot;

	FTimerHandle DeathTimer;
	float DeathTimout = 5.f;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetOverlay(UOverlayWidget* InOverlay) { Overlay = InOverlay; }
	FORCEINLINE bool IsAlive() const { return bAlive; }
};
