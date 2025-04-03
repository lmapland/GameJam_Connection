// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionObject.h"
#include "ConnectionBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnect, int32, InLevel);

class UStaticMeshComponent;
class UBoxComponent;
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
	virtual void SetReady() override;
	void SetConnected();
	void BoxesAreConnected();
	void Fill();
	virtual FVector GetBeamAttachPoint() override;

	UFUNCTION(BlueprintImplementableEvent)
	void Stop() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetParticleState(bool bElectrified, bool bConnected);

	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	//void SetVisuallyFilled();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	USceneComponent* DefaultComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Properties")
	UStaticMeshComponent* ConnectedBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Properties")
	UBoxComponent* AreaBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	int32 Level;

	UPROPERTY()
	FOnConnect OnConnectDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Properties")
	UNiagaraSystem* ConnectedParticles;
	
	/*UPROPERTY(EditAnywhere, Category = "Box Properties")
	USoundBase* UseInitialSound;*/
	
	UPROPERTY(EditAnywhere, Category = "Box Properties")
	USoundBase* UseCompleteSound;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Properties")
	FVector BeamEnd = FVector(0.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box Properties")
	FVector BeamEndUnconnected = FVector(0.f);

private:
	void SpawnParticles(UNiagaraSystem* ParticlesToSpawn, FVector LocationToSpawn);
	void PlaySound(USoundBase* SoundToPlay);

	UFUNCTION(BlueprintCallable)
	void ResetState();

	/*
	* if Connected, cannot be connected again
	* if Unconnected, cannot be Filled, even if Ready
	* if Connected AND Ready, should be filled
	*/

	/* Internally, junction is not Connected */
	bool bIsConnected = false;
};
