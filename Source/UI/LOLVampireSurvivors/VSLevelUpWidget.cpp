// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LOLVampireSurvivors/VSLevelUpWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Attribute/VSCharacterAttributeSet.h"
#include "Player/VSPlayerState.h"

#include "ProjectLOL.h"

UVSLevelUpWidget::UVSLevelUpWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UVSLevelUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//SkillButtonList.Add(Cast<UButton>(GetWidgetFromName(TEXT("SkillButton0"))));
	//SkillButtonList.Add(Cast<UButton>(GetWidgetFromName(TEXT("SkillButton1"))));
	//SkillButtonList.Add(Cast<UButton>(GetWidgetFromName(TEXT("SkillButton2"))));

	//LevelUpPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("LevelUpPanel")));

	//UVSCharacterAttributeSet* CharacterAttributeSet = VSPlayerState.Get()->GetCharacterAttributeSet();
	//CharacterAttributeSet->OnLevelUp.AddDynamic(this, &UVSLevelUpWidget::UpdateSkiils);

	Hide();
}

void UVSLevelUpWidget::Show()
{
	LevelUpPanel->SetVisibility(ESlateVisibility::Visible);
}

void UVSLevelUpWidget::Hide()
{
	LevelUpPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UVSLevelUpWidget::UpdateSkiils()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Show();
}
