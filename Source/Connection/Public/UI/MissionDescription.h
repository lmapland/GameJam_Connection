// Copyright Leslie Apland

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionDescription.generated.h"

class AXtionsHUD;

/**
 * 
 */
UCLASS()
class CONNECTION_API UMissionDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(AXtionsHUD* InHUD);

protected:
	UFUNCTION(BlueprintCallable)
	void Continue();

private:
	AXtionsHUD* HUD;
};
