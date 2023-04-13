// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/XtionsCharacter.h"
#include "Enemies/Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	MyEnemy = UGameplayStatics::GetActorOfClass(this, AXtionsCharacter::StaticClass());

	if (ForeverParticles)
	{
		//UNiagaraComponent* ForeverComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ForeverParticles, GetActorLocation() - FVector(0.f, 0.f, 120.f));
		ForeverComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ForeverParticles, GetActorLocation() - FVector(0.f, 0.f, 120.f));
	}

	GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BetweenFiring);
}

void AEnemy::PostDie()
{
	ForeverComponent->DestroyComponent();
	Destroy();
}

void AEnemy::CanFire()
{
	if (!bAlive) return;

	if (GetDistanceTo(MyEnemy) <= MaxFireDistance)
	{
		bInterpToCharacter = true;

		//UE_LOG(LogTemp, Warning, TEXT("AEnemy::CanFire(): Firing"));

		// execute Montage
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && FireMontage)
		{
			AnimInstance->Montage_Play(FireMontage);
			AnimInstance->Montage_JumpToSection(FName("Default"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemy::CanFire(): GetDistanceTo(MyEnemy): %f"), GetDistanceTo(MyEnemy));
		GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BetweenFiring);
	}
}

AProjectile* AEnemy::SpawnProjectile()
{
	FVector LocationToSpawn = GetActorLocation() + FVector(0.f, 0.f, 40.f) + (GetActorForwardVector() * 90);
	return GetWorld()->SpawnActor<AProjectile>(ProjectileClass, LocationToSpawn, GetActorRotation());
}

void AEnemy::SetInterpToCharacter(bool Interp)
{
	bInterpToCharacter = Interp;
}

void AEnemy::Fire()
{
	if (!bAlive) return;

	bInterpToCharacter = false;

	//UE_LOG(LogTemp, Warning, TEXT("AEnemy::Fire(): Called"));

	// execute on-fire event effect
	if (FireParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireParticles, GetActorLocation());
	}
	
	// execute sound

	AProjectile* Projectile = SpawnProjectile();

	// Call timer to start firing again
	if (GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemy::Fire(): FireTimer is currently active. Clearing"));
		GetWorldTimerManager().ClearTimer(FireTimer);
	}
	else
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BetweenFiring);
		//UE_LOG(LogTemp, Warning, TEXT("AEnemy::Fire(): FireTimer set"));
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterpToCharacter)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MyEnemy->GetActorLocation());
		FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotationYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}
}

void AEnemy::Die()
{
	bAlive = false;

	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();

	FireTimer.Invalidate();

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::PostDie, UntilDisappear);
}

