// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverlayWidget.h"
#include "Components/TextBlock.h"

void UOverlayWidget::UpdateLives(int32 Amount)
{
	if (Amount < 0)
	{
		PlayerHurt();
	}
	else
	{
		PlayerGainedHealth();
	}
}
