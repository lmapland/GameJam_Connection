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

	BeamEndComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BeamEnd"));
	BeamEndComponent->SetupAttachment(GetRootComponent());
}

void AConnectionLine::BeginPlay()
{
	Super::BeginPlay();

	/*if (bIsFirst && bIsReady)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConnectionLine::BeginPlay(): setting bIsOriginally ready to true for %s"), *GetName());
		bIsOriginallyReady = bIsReady;
		Fill();
	}*/
}

void AConnectionLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConnectionLine::SetReady()
{
	//bIsOriginallyReady = bIsReady;
	//UE_LOG(LogTemp, Warning, TEXT("ConnectionLine::SetReady(): %s"), *GetName());
	// Don't unnecessarily "fill" self and call Next if that's already happened
	/*if (bIsReady)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConnectionLine::SetReady(): bIsReady already; exiting"));
		return;
	}*/

	// Ready to be filled. A line will always Fill when the previous connection is Filled, so we just do the thing here
	//UE_LOG(LogTemp, Warning, TEXT("ConnectionLine::SetReady(): bIsReady is false; calling Fill()"));
	//bIsReady = true;
	Fill();
}

void AConnectionLine::Fill()
{
	if (NextPiece)
	{
		BeamEnd = NextPiece->GetBeamAttachPoint();
		SetVisuallyFilled();
		NextPiece->SetReady();
	}
}

FVector AConnectionLine::GetBeamAttachPoint()
{
	return BeamEndComponent->GetComponentLocation();
	//return ConnectedLine->GetSocketLocation(FName("BeamEnd"));
}

void AConnectionLine::Prepare()
{
	//UE_LOG(LogTemp, Warning, TEXT("ConnectionLine::Prepare(): Object name: %s"), *GetName());
	if (bIsFirst && bIsReady)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConnectionLine::Prepare(): %s Filling"), *GetName());
		Fill();
	}
}
