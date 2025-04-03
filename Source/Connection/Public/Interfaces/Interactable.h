// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CONNECTION_API IInteractable
{
	GENERATED_BODY()

public:
	virtual FString GetInteractableName();
	virtual int32 GetInteractableID();
	virtual void Interact() = 0;

};
