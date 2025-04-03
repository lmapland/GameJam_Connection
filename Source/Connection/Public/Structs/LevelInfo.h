#pragma once

#include "CoreMinimal.h"
#include "LevelInfo.generated.h"

USTRUCT()
struct FLevelInfo
{
	GENERATED_BODY()

	int32 MaxHits = 4;
	int32 Connections = 1;
	int32 Dodges = 10;
	int32 Jumps = 10;
	FVector Location;
	FRotator Rotation;
	TArray<int32> RequiredObjects;
	TArray<int32> ObjectCounts;
};
