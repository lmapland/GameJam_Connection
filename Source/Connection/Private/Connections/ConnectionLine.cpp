// Fill out your copyright notice in the Description page of Project Settings.


#include "Connections/ConnectionLine.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AConnectionLine::AConnectionLine()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	SetRootComponent(DefaultComponent);

	ConnectedLine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectedLine"));
	ConnectedLine->SetupAttachment(GetRootComponent());
}

void AConnectionLine::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConnectionLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConnectionLine::SetReady()
{
	// Ready to be filled. A line will always Fill when the previous connection is Filled, so we just do the thing here
	Fill();
}

void AConnectionLine::Fill()
{
	bIsFilled = true;
	SetMaterial();

	// change material
	if (NextPiece)
	{
		NextPiece->SetReady();
	}
}

