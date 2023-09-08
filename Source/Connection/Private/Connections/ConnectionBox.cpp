// Fill out your copyright notice in the Description page of Project Settings.


#include "Connections/ConnectionBox.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/XtionsCharacter.h"
#include "Enemies/Enemy.h"

AConnectionBox::AConnectionBox()
{
	PrimaryActorTick.bCanEverTick = true;

	ConnectedBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectedBox"));
	SetRootComponent(ConnectedBox);

	UnconnectedBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnconnectedBox"));
	UnconnectedBox->SetupAttachment(GetRootComponent());

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
}

void AConnectionBox::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AConnectionBox::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AConnectionBox::OnSphereEndOverlap);
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
	if (!bIsUnconnected) return false;
	bIsUnconnected = false;

	OnConnectDelegate.Broadcast(Level);
	ConnectBoxes();
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	if (ConnectedParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ConnectedParticles, GetActorLocation() + FVector(0.f, 0.f, 60.f));
	}
}

void AConnectionBox::SetReady()
{
	bIsReady = true;
	if (!bIsUnconnected) // Connected
	{
		Fill();
	}
}

void AConnectionBox::SetConnected()
{
	if (bIsReady)
	{
		Fill();
	}
}

void AConnectionBox::Fill()
{
	bIsFilled = true;
	FillWithColor();

	if (NextPiece)
	{
		NextPiece->SetReady();
	}
}
