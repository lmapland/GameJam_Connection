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
	void SimpleFire();

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
	AProjectile* SpawnProjectile(FVector LocationToSpawn = FVector(0.f));
	void PlaySound(USoundBase* SoundToPlay);

	FTimerHandle DeathTimer;
	float UntilDisappear = 4.f;

	FTimerHandle FireTimer;

	/* Amount of time to wait between firing projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	float BetweenFiring = 2.f;

	/* Amount of time to wait between when the character comes into play and the enemy begins firing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	float BeginPlayFiring = 0.5f;

	/* Should the enemy fire two projectiles at the same time? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	bool bFireTwoAtATime = false;

	/* Where should the second projectile be fired from? This var is not used unless bFireTwoAtATime is true */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	FVector SecondProjectileOffset = FVector(0.f);

	/* Should the enemy fire two projectiles quickly in sequence? This causes a different animation to be played */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Enemy|Combat")
	bool bFireTwoSequentially = false;

	float InterpSpeed = 10.f;
	bool bInterpToCharacter = false;

	bool bAlive = true;

	UNiagaraComponent* ForeverComponent;
};
