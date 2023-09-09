// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CanTurnOn.h"
#include "Enemy.generated.h"

class AXtionsCharacter;
class UNiagaraSystem;
class AProjectile;
class UNiagaraComponent;
class USoundBase;

UCLASS()
class CONNECTION_API AEnemy : public ACharacter, public ICanTurnOn
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Fire();

	virtual void Start() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	float MaxFireDistance = 4000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	AXtionsCharacter* MyEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
	UNiagaraSystem* ForeverParticles;
	
	UPROPERTY(EditAnywhere, Category = "Enemy|Attributes")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	int32 Level = 0;

protected:
	virtual void BeginPlay() override;

private:
	void PostDie();
	void CanFire();
	AProjectile* SpawnProjectile();
	void SetInterpToCharacter(bool Interp);
	void PlaySound(USoundBase* SoundToPlay);

	FTimerHandle DeathTimer;
	float UntilDisappear = 4.f;

	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	float BetweenFiring = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	float BeginPlayFiring = 0.5f;

	float InterpSpeed = 10.f;
	bool bInterpToCharacter = false;

	bool bAlive = true;

	UNiagaraComponent* ForeverComponent;
};
