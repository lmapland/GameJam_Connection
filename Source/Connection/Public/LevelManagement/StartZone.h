// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CanTurnOn.h"
#include "StartZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterExit, int32, InLevel);

class UBoxComponent;

UCLASS()
class CONNECTION_API AStartZone : public AActor, public ICanTurnOn
{
	GENERATED_BODY()
	
public:
	AStartZone();
	virtual void Tick(float DeltaTime) override;
	void CharacterExited(int32 InLevel);
	virtual void Prepare() override;
	
	UPROPERTY()
	FOnCharacterExit OnCharacterExitDelegate;

	float StartLocationZ;
	float DistanceToDrop = 10000.f;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 Level = 0;

public:
	FORCEINLINE int32 GetLevel() const { return Level; }
};
