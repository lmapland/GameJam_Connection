// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AXtionsCharacter;
class UNiagaraSystem;
class AProjectile;
class UNiagaraComponent;

UCLASS()
class CONNECTION_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MaxFireDistance = 4000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AActor* MyEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UNiagaraSystem* ForeverParticles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UNiagaraSystem* FireParticles; // Executed immediately when fired
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	//UNiagaraSystem* TrailParticles; // these might be part of the projectile, actually
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UNiagaraSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float ProjectileForce = 3000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 Level = 0;

protected:
	virtual void BeginPlay() override;

private:
	void PostDie();
	void CanFire();
	AProjectile* SpawnProjectile();
	void SetInterpToCharacter(bool Interp);

	FTimerHandle DeathTimer;
	float UntilDisappear = 4.f;

	FTimerHandle FireTimer;
	float BetweenFiring = 2.f;
	float BetweenFiringLong = 3.5f;

	float InterpSpeed = 10.f;
	bool bInterpToCharacter = false;

	bool bAlive = true;

	UNiagaraComponent* ForeverComponent;
};
