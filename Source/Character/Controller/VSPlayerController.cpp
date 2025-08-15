// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LOLVampireSurvivors/Controller/VSPlayerController.h"
#include "VSPlayerController.h"
#include "UI/LOLVampireSurvivors/VSHUDWidget.h"

AVSPlayerController::AVSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UVSHUDWidget> VSHUDWidgetRef(TEXT("/Game/UI/LOLVampireSurvivors/VSBBP_HUD.VSBBP_HUD_C"));
	if (VSHUDWidgetRef.Class)
	{
		VSHUDWidgetClass = VSHUDWidgetRef.Class;
	}
}

void AVSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// set inputmode cursor in game. 
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// show cursor in game.
	SetShowMouseCursor(true);
}
