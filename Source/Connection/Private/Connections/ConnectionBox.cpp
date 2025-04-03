// Fill out your copyright notice in the Description page of Project Settings.


#include "Connections/ConnectionBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/XtionsCharacter.h"
#include "Enemies/Enemy.h"

AConnectionBox::AConnectionBox()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	SetRootComponent(DefaultComponent);

	ConnectedBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectedBox"));
	ConnectedBox->SetupAttachment(GetRootComponent());

	AreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaBox"));
	AreaBox->SetupAttachment(GetRootComponent());

	BeamEndComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BeamEnd"));
	BeamEndComponent->SetupAttachment(GetRootComponent());
}

void AConnectionBox::BeginPlay()
{
	Super::BeginPlay();

	AreaBox->OnComponentBeginOverlap.AddDynamic(this, &AConnectionBox::OnSphereOverlap);
	AreaBox->OnComponentEndOverlap.AddDynamic(this, &AConnectionBox::OnSphereEndOverlap);

	bIsOriginallyReady = bIsReady;

	if (NextPiece)
	{
		BeamEnd = NextPiece->GetBeamAttachPoint();
		BeamEndUnconnected = GetActorForwardVector() * (FVector::DistXY(BeamEnd, GetActorLocation()) / 2) + GetActorLocation();
		BeamEndUnconnected.Z -= 100.f;
	}

	if (bIsFirst && bIsReady)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BeginPlay(): SetParticleStateReady being called on %s, Beam End: %f, %f"), *GetName(), BeamEndUnconnected.X, BeamEndUnconnected.Y);
		SetParticleState(bIsReady, bIsConnected);
	}
}

void AConnectionBox::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AXtionsCharacter* Char = Cast<AXtionsCharacter>(OtherActor);
	if (Char)
	{
		Char->SetOverlappedConnectionBox(this);
	}
}

void AConnectionBox::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AXtionsCharacter* Char = Cast<AXtionsCharacter>(OtherActor);
	if (Char)
	{
		Char->SetOverlappedConnectionBox(nullptr);
	}
}

void AConnectionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AConnectionBox::Use()
{
	if (bIsConnected) return false;
	bIsConnected = true;

	OnConnectDelegate.Broadcast(Level);
	BoxesAreConnected();
	AreaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//PlaySound(UseInitialSound);

	return true;
}

void AConnectionBox::BoxesAreConnected()
{
	// kill all enemies of appropriate type
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(this, EnemyClass, AllEnemies);

	for (auto CurrentActor : AllEnemies)
	{
		if (AEnemy* CurrentEnemy = Cast<AEnemy>(CurrentActor))
		{
			// Get the enemy's level and compare it to this instance's level
			if (CurrentEnemy->Level == Level)
			{
				CurrentEnemy->Die();
			}
		}
	}
	SetConnected();

	SpawnParticles(ConnectedParticles, GetActorLocation() + FVector(0.f, 0.f, 60.f));
	PlaySound(UseCompleteSound);
}

void AConnectionBox::SetReady()
{
	//UE_LOG(LogTemp, Warning, TEXT("ConnectionBox::SetReady(): %s"), *GetName());
	bIsReady = true;
	if (bIsConnected)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ConnectionBox::SetReady(): Is Connected; filling"));
		Fill();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("ConnectionBox::SetReady(): is NOT connected; SetParticleStateReady called on %s, Beam End: %f, %f"), *GetName(), BeamEndUnconnected.X, BeamEndUnconnected.Y);
		SetParticleState(bIsReady, bIsConnected);
	}
}

void AConnectionBox::SetConnected()
{
	if (bIsReady)
	{
		Fill();
	}
}

void AConnectionBox::SpawnParticles(UNiagaraSystem* ParticlesToSpawn, FVector LocationToSpawn)
{
	if (ParticlesToSpawn)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ParticlesToSpawn, LocationToSpawn);
	}
}

void AConnectionBox::PlaySound(USoundBase* SoundToPlay)
{
	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
	}
}

void AConnectionBox::ResetState()
{
	//UE_LOG(LogTemp, Warning, TEXT("AConnectionBox::ResetState(): Setting bIsConnected to false"));
	bIsReady = bIsOriginallyReady;
	bIsConnected = false;
	AreaBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AConnectionBox::Fill()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fill(): SetParticleStateReady called on %s, Beam End: %f, %f"), *GetName(), BeamEndUnconnected.X, BeamEndUnconnected.Y);
	SetParticleState(bIsReady, bIsConnected);

	if (bIsConnected && NextPiece)
	{
		NextPiece->SetReady();
	}
}

FVector AConnectionBox::GetBeamAttachPoint()
{
	return BeamEndComponent->GetComponentLocation();
	//return ConnectedBox->GetSocketLocation(FName("BeamEnd"));
}
