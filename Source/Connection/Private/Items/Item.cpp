// Copyright Leslie Apland


#include "Items/Item.h"
#include "Components/MeshComponent.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(GetRootComponent());

}

FString AItem::GetInteractableName()
{
	return InteractableName;
}

int32 AItem::GetInteractableID()
{
	return ID;
}

void AItem::Interact()
{
	PlayPickupSound();
	Stop();
}

void AItem::Prepare()
{
	SetActorLocation(OriginalLocation);
}

void AItem::Start()
{
}

void AItem::Stop()
{
	SetActorLocation(GetActorLocation() - FVector(0.f, 0.f, 2000.f));
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalLocation = GetActorLocation();
}

void AItem::PlayPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
}
