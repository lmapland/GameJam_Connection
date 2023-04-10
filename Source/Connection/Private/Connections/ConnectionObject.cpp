// Fill out your copyright notice in the Description page of Project Settings.


#include "Connections/ConnectionObject.h"

AConnectionObject::AConnectionObject()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AConnectionObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConnectionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConnectionObject::SetReady()
{
}

