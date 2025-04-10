// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/XtionsCharacter.h"
#include "Enemies/Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	this->SetActorTickEnabled(false);

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* EnemyActor = UGameplayStatics::GetActorOfClass(this, AXtionsCharacter::StaticClass());
	if (EnemyActor) MyEnemy = Cast<AXtionsCharacter>(EnemyActor);

	if (ForeverParticles)
	{
		ForeverComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ForeverParticles, GetActorLocation() - FVector(0.f, 0.f, 120.f));
	}

	StartingLocation = GetMesh()->GetRelativeLocation();
	StartingRotation = GetMesh()->GetRelativeRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Starting rotation: %f, %f, %f"), StartingRotation.Yaw, StartingRotation.Pitch, StartingRotation.Roll);
}

void AEnemy::PostDie()
{
	this->SetActorTickEnabled(false);
	ForeverComponent->SetVisibility(false);
	EndElectricParticles();
	GetMesh()->SetVisibility(false);
}

void AEnemy::CanFire()
{
	if (!bAlive || !MyEnemy->IsAlive()) return;

	if (!CanSeeMyEnemy())
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BetweenFiring);
		return;
	}

	if (GetDistanceTo(MyEnemy) <= MaxFireDistance)
	{
		// execute Montage
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && FireMontage)
		{
			AnimInstance->Montage_Play(FireMontage);
			if (bFireTwoSequentially)
			{
				AnimInstance->Montage_JumpToSection(FName("DoubleFire"));
			}
			else
			{
				AnimInstance->Montage_JumpToSection(FName("Default"));
			}
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEnemy::CanFire(): GetDistanceTo(MyEnemy): %f"), GetDistanceTo(MyEnemy));
		GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BetweenFiring);
	}
}

AProjectile* AEnemy::SpawnProjectile(FVector LocationToSpawn)
{
	const FVector SpawnLocation = GetActorLocation() + LocationToSpawn + FVector(0.f, 0.f, 40.f);
	const FVector ToEnemyNormalized = ((MyEnemy->GetActorLocation() + FVector(0.f,0.f,50.f)) - SpawnLocation).GetSafeNormal();
	const FRotator StartRotation = ToEnemyNormalized.ToOrientationRotator();
	return GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, StartRotation);
}

void AEnemy::SimpleFire()
{
	if (!bAlive || !MyEnemy->IsAlive()) return;

	AProjectile* Projectile = SpawnProjectile();

	if (bFireTwoAtATime)
	{
		SpawnProjectile(SecondProjectileOffset);
	}

	PlaySound(FireSound);
}

void AEnemy::Fire()
{
	if (!bAlive || !MyEnemy->IsAlive()) return;

	AProjectile* Projectile = SpawnProjectile();

	if (bFireTwoAtATime)
	{
		SpawnProjectile(SecondProjectileOffset);
	}

	PlaySound(FireSound);

	// Call timer to start firing again
	if (GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		GetWorldTimerManager().ClearTimer(FireTimer);
	}
	else
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BetweenFiring);
	}
}

void AEnemy::Prepare()
{
	//UE_LOG(LogTemp, Warning, TEXT("AEnemy::Prepare(): %s"), *GetName());
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->PutAllRigidBodiesToSleep();

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), AttachmentRules);
	GetMesh()->SetRelativeLocation(StartingLocation);
	GetMesh()->SetRelativeRotation(StartingRotation);
	
	GetWorld()->GetTimerManager().SetTimer(PostPrepareHandle, this, &AEnemy::FinishPrepare, PostPrepareTime);
	//ForeverComponent->SetVisibility(true);
	//GetMesh()->SetVisibility(true);
}

void AEnemy::FinishPrepare()
{
	GetRootComponent()->SetVisibility(true, true);
}

void AEnemy::Start()
{
	GetRootComponent()->SetVisibility(true, true);
	bAlive = true;
	this->SetActorTickEnabled(true);
	GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemy::CanFire, BeginPlayFiring);
}

void AEnemy::Stop()
{
	bAlive = false;
	FireTimer.Invalidate();

	this->SetActorTickEnabled(false);
	ForeverComponent->SetVisibility(false);
	GetMesh()->SetVisibility(false);
}

bool AEnemy::CanSeeMyEnemy()
{
	FHitResult HitResult;

	// how to get the camera location & rotation in world space
	const FVector SpawnLocation = GetActorLocation();
	const FVector ToEnemyNormalized = ((MyEnemy->GetActorLocation() + FVector(0.f, 0.f, 50.f)) - SpawnLocation).GetSafeNormal();
	const FVector EndLocation = SpawnLocation + ToEnemyNormalized * MaxFireDistance;

	GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, EndLocation, ECollisionChannel::ECC_Visibility);
	//DrawDebugLine(GetWorld(), SpawnLocation, EndLocation, FColor::Black, true, 60.f, 0, 1.f);

	if (HitResult.bBlockingHit)
	{
		if (MyEnemy == Cast<AXtionsCharacter>(HitResult.GetActor()))
		{
			return true;
		}
	}
	return false;
}

void AEnemy::PlaySound(USoundBase* SoundToPlay)
{
	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
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
	StartElectricParticles();

	FireTimer.Invalidate();

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::PostDie, UntilDisappear);
}

