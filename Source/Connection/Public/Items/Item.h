// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Interfaces/CanTurnOn.h"
#include "Item.generated.h"

class USoundBase;
class UStaticMeshComponent;

UCLASS()
class CONNECTION_API AItem : public AActor, public IInteractable, public ICanTurnOn
{
	GENERATED_BODY()
	
public:
	AItem();
	virtual FString GetInteractableName() override;
	virtual int32 GetInteractableID() override;
	virtual void Interact() override;

	virtual void Prepare() override;
	virtual void Start() override;
	virtual void Stop() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

private:
	void PlayPickupSound();

	UPROPERTY(EditAnywhere, Category = "Item|Sounds")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, Category = "Item|Interactable")
	FString InteractableName;

	UPROPERTY(EditAnywhere, Category = "Item|Interactable")
	int32 ID = 1;

	FVector OriginalLocation;
};
