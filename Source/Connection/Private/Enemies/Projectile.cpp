// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/XtionsCharacter.h"
#include "Enemies/Enemy.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(true);
	ProjectileMesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (FireballParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(FireballParticles, GetMesh(), FName(""), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor)) return;
	if (OtherActor == this) return;

	if (AXtionsCharacter* Char = Cast<AXtionsCharacter>(OtherActor))
	{
		Char->Damage(DamageAmount);
	}

	Explode();
	Destroy();
}

void AProjectile::Explode()
{
	if (ExplodeParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplodeParticles, GetActorLocation());
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
