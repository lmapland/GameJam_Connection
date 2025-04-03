// Copyright Leslie Apland


#include "GoodSky/GoodSkyBase.h"
#include "GameInstances/XtionsGameInstance.h"
#include "LevelManagement/LevelManager.h"

AGoodSkyBase::AGoodSkyBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGoodSkyBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(BeginPlayTimerHandle, this, &AGoodSkyBase::FinishBeginPlay, BeginPlayTime);

}

void AGoodSkyBase::FinishBeginPlay()
{
	UXtionsGameInstance* GameInstance = Cast<UXtionsGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGoodSkyBase::BeginPlay(): Could not find GI"));
	}
	else
	{
		ULevelManager* LevelManager = GameInstance->GetLevelManager();
		if (LevelManager)
		{
			LevelManager->OnStartLevel.AddDynamic(this, &AGoodSkyBase::StartLevel);
		}
	}
}
