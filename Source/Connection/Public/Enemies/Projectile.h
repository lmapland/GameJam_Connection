// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class CONNECTION_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	void FireInDirection(const FVector& ShootDirection);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Properties")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Properties")
	int32 DamageAmount = 1;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	void Explode();

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return ProjectileMesh; }

};
