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
	Destroy();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::PlayPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
}
