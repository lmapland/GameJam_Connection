// Copyright Leslie Apland


#include "UI/XtionsHUD.h"
#include "Characters/XtionsCharacter.h"
#include "Widgets/OverlayWidget.h"
#include "Widgets/Controllers/OverlayWidgetController.h"

void AXtionsHUD::InitOverlay(AXtionsCharacter* InCharacter, ULevelManager* InLevelManager)
{
	if (!OverlayClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD::InitOverlay(): OverlayClass is not set!"));
		return;
	}

	UUserWidget* OverlayWidget = CreateWidget<UUserWidget>(GetWorld(), OverlayClass);
	Overlay = Cast<UOverlayWidget>(OverlayWidget);
	OverlayController = NewObject<UOverlayWidgetController>();

	OverlayController->SetWidgetControllerParams(InCharacter, InLevelManager);
	Overlay->SetController(OverlayController);
	OverlayController->BindCallbacksToDependencies();
	OverlayController->BroadcastInitialValues();
	Overlay->AddToViewport();
}
