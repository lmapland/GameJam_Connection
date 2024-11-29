// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionObject.h"
#include "ConnectionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnect, int32, InLevel);

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class CONNECTION_API AConnectionBox : public AConnectionObject
{
	GENERATED_BODY()
	
public:
	AConnectionBox();
	virtual void Tick(float DeltaTime) override;
	bool Use();

	UFUNCTION(BlueprintImplementableEvent)
	void ConnectBoxes();

	UFUNCTION(BlueprintCallable)
	void BoxesAreConnected();

	void Fill();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FillWithColor();

	virtual void SetReady() override;
	void SetConnected();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	UStaticMeshComponent* ConnectedBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	UStaticMeshComponent* UnconnectedBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	int32 Level;

	UPROPERTY()
	FOnConnect OnConnectDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	UNiagaraSystem* ConnectedParticles;
	
	UPROPERTY(EditAnywhere, Category = "Box Properties")
	USoundBase* UseInitialSound;
	
	UPROPERTY(EditAnywhere, Category = "Box Properties")
	USoundBase* UseCompleteSound;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void SpawnParticles(UNiagaraSystem* ParticlesToSpawn, FVector LocationToSpawn);
	void PlaySound(USoundBase* SoundToPlay);

	/*
	* if Connected, cannot be connected again
	* if Unconnected, cannot be Filled, even if Ready
	* if Connected AND Ready, should be filled
	*/

	/* Internally, junction is Unconnected */
	bool bIsUnconnected = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFilled = false;
};
