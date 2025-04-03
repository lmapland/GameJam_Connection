// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CanTurnOn.h"
#include "ConnectionObject.generated.h"

UCLASS()
class CONNECTION_API AConnectionObject : public AActor, public ICanTurnOn
{
	GENERATED_BODY()
	
public:
	AConnectionObject();
	virtual void Tick(float DeltaTime) override;
	virtual void SetReady();
	virtual FVector GetBeamAttachPoint();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Connection Properties")
	AConnectionObject* NextPiece;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	USceneComponent* BeamEndComponent;

protected:
	virtual void BeginPlay() override;

	/* Indicates whether previous piece of chain is Filled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connection Properties")
	bool bIsReady = false;

	/* Should be one First / Start piece per level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connection Properties")
	bool bIsFirst = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Connection Properties")
	bool bIsOriginallyReady = false;
};
