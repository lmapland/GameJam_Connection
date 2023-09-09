// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class CONNECTION_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Properties")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Properties")
	int32 DamageAmount = 1;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	UNiagaraSystem* ExplodeParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties")
	UNiagaraSystem* FireballParticles;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	USoundBase* ExplodeSound;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	void Explode();
	void SpawnParticles(UNiagaraSystem* ParticlesToSpawn, FVector LocationToSpawn, bool bIsAttached = false);
	void PlaySound(USoundBase* SoundToPlay);

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return ProjectileMesh; }

};
