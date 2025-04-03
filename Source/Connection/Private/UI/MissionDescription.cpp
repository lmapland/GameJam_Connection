// Copyright Leslie Apland


#include "UI/MissionDescription.h"
#include "UI/XtionsHUD.h"

void UMissionDescription::Setup(AXtionsHUD* InHUD)
{
	HUD = InHUD;
}

void UMissionDescription::Continue()
{
	HUD->ShowLevelSelectionScreen();
}
