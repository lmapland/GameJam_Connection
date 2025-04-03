// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionObject.h"
#include "ConnectionLine.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class CONNECTION_API AConnectionLine : public AConnectionObject
{
	GENERATED_BODY()
	
public:
	AConnectionLine();
	virtual void Tick(float DeltaTime) override;
	virtual void SetReady() override;
	void Fill();
	virtual FVector GetBeamAttachPoint() override;
	virtual void Prepare() override;

	UFUNCTION(BlueprintImplementableEvent)
	void Stop() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetVisuallyFilled();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line Properties")
	UStaticMeshComponent* ConnectedLine;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line Properties")
	USceneComponent* DefaultComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line Properties")
	FVector BeamEnd = FVector(0.f);
};
