// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanTurnOn.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanTurnOn : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CONNECTION_API ICanTurnOn
{
	GENERATED_BODY()

public:
	/** Called when the level visibly loads
	 * this function should cover everything that that needs to happen or look correct
	 * before the player moves off of the start zone and begins the game
	 */
	virtual void Prepare();

	/* Called when the player steps off the start zone */
	virtual void Start();

	/** Called when the player leaves the level
	 * either because they completed it or opened the level selection screen
	 */
	virtual void Stop();
};
