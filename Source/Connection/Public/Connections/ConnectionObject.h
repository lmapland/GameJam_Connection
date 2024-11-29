// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConnectionObject.generated.h"

UCLASS()
class CONNECTION_API AConnectionObject : public AActor
{
	GENERATED_BODY()
	
public:
	AConnectionObject();
	virtual void Tick(float DeltaTime) override;
	virtual void SetReady();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Connection Properties")
	AConnectionObject* NextPiece;


protected:
	virtual void BeginPlay() override;

	/* Indicates whether previous piece of chain is Filled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line Properties")
	bool bIsReady = false;

};
