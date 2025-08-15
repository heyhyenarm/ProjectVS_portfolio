// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/LOLVampireSurvivors/VSLevelUpManager.h"
#include "GameData/LOLVampireSurvivors/VSChampionSkillDataAsset.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"

#include "UI/LOLVampireSurvivors/VSHUDWidget.h"
#include "UI/LOLVampireSurvivors/VSLevelUpWidget.h"

#include "ProjectLOL.h"

void UVSLevelUpManager::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UVSLevelUpManager::OnLevelUp(AVSCharacterChampionPlayer* Champion)
{
	//LOL_LOG(LogProjectLOL, Warning, TEXT("Level Up"));

	// 랜덤 스킬/업그레이드 선정하기. 
	// DataAsset에서 랜덤하게 스킬/업그레이드 가져오기. 


	// @NHRTodo : UI 레벨업 로직. 
	// 임시 랜덤 부여. 

	HUDWidget->LevelUpWidget->UpdateSkiils();

}

void UVSLevelUpManager::SetupHUDWidget(UVSHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

		HUDWidget = InHUDWidget;


		//Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
		//Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
	}
}
