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

	// ���� ��ų/���׷��̵� �����ϱ�. 
	// DataAsset���� �����ϰ� ��ų/���׷��̵� ��������. 


	// @NHRTodo : UI ������ ����. 
	// �ӽ� ���� �ο�. 

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
