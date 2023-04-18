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
	virtual void Start();
};
