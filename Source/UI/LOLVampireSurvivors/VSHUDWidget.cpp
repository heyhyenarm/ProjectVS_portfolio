// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LOLVampireSurvivors/VSHUDWidget.h"

#include "Components/Overlay.h"

#include "SubSystem/LOLVampireSurvivors/VSLevelUpManager.h"
#include "SubSystem/LOLVampireSurvivors/VSGameSubsystem.h"

#include "ProjectLOL.h"

UVSHUDWidget::UVSHUDWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UVSHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetSubsystem<UVSLevelUpManager>()->SetupHUDWidget(this);
	GetWorld()->GetSubsystem<UVSGameSubsystem>()->OnGameOverDelegate.AddDynamic(this, &UVSHUDWidget::OnGameOver);
}

bool UVSHUDWidget::Initialize()
{
	Super::Initialize();
	
	GameOverPanel->SetVisibility(ESlateVisibility::Collapsed);

	return true;
}

void UVSHUDWidget::OnGameOver()
{
	LOL_LOG(LogProjectLOL, Error, TEXT("GAME OVER!!!!!!!!!!!!!!!!!!!!!!!!!"));
	GameOverPanel->SetVisibility(ESlateVisibility::Visible);
}
