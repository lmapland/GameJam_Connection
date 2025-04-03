// Copyright Leslie Apland


#include "UI/XtionsHUD.h"
#include "Characters/XtionsCharacter.h"
#include "Widgets/OverlayWidget.h"
#include "UI/LevelSelection.h"
#include "UI/MissionDescription.h"
#include "Widgets/Controllers/OverlayWidgetController.h"

void AXtionsHUD::InitOverlay(AXtionsCharacter* InCharacter, ULevelManager* InLevelManager)
{
	if (!OverlayClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD::InitOverlay(): OverlayClass is not set!"));
		return;
	}

	if (!LevelSelectionClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD::InitOverlay(): LevelSelectionClass is not set!"));
		return;
	}

	Character = InCharacter;

	UUserWidget* LevelSelectWidget = CreateWidget<UUserWidget>(GetWorld(), LevelSelectionClass);
	SelectLevelScreen = Cast<ULevelSelection>(LevelSelectWidget);
	SelectLevelScreen->Setup(InLevelManager);

	UUserWidget* MissionDescWidget = CreateWidget<UUserWidget>(GetWorld(), PreGameMissionDescClass);
	MissionDescription = Cast<UMissionDescription>(MissionDescWidget);
	MissionDescription->Setup(this);

	OverlayController = NewObject<UOverlayWidgetController>();
	OverlayController->SetWidgetControllerParams(InCharacter, InLevelManager);
	OverlayController->BindCallbacksToDependencies();

	UUserWidget* OverlayWidget = CreateWidget<UUserWidget>(GetWorld(), OverlayClass);
	Overlay = Cast<UOverlayWidget>(OverlayWidget);
	Overlay->SetController(OverlayController);
	OverlayController->BroadcastInitialValues();

	MissionDescription->AddToViewport();
	MissionDescription->SetFocus();
}

void AXtionsHUD::SetShowOverlay(bool bShowOverlay)
{
	if (bOverlayIsShown && !bShowOverlay) // turn overlay off
	{
		Overlay->RemoveFromViewport();
		bOverlayIsShown = false;
	}
	else if (!bOverlayIsShown && bShowOverlay) // turn overlay on
	{
		/*if (!Overlay)
		{

		}*/
		Overlay->AddToViewport();
		bOverlayIsShown = true;
	}
}

void AXtionsHUD::ShowLevelSelectionScreen()
{
	SetShowOverlay(false);
	SelectLevelScreen->AddToViewport();
	SelectLevelScreen->SetFocus();

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
}
