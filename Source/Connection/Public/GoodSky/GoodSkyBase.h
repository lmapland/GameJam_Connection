// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoodSkyBase.generated.h"

UCLASS()
class CONNECTION_API AGoodSkyBase : public AActor
{
	GENERATED_BODY()
	
public:
	AGoodSkyBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartLevel(int32 InLevel);

private:
	void FinishBeginPlay();

	FTimerHandle BeginPlayTimerHandle;
	float BeginPlayTime = 0.25f;
};
