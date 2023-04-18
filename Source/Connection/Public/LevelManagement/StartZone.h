// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterExit, int32, InLevel);

class UBoxComponent;

UCLASS()
class CONNECTION_API AStartZone : public AActor
{
	GENERATED_BODY()
	
public:
	AStartZone();
	virtual void Tick(float DeltaTime) override;
	void CharacterExited(int32 InLevel);
	
	UPROPERTY()
	FOnCharacterExit OnCharacterExitDelegate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	int32 Level = 0;

public:
	FORCEINLINE int32 GetLevel() const { return Level; }
};
