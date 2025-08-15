// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/LOLVampireSurvivors/VSAttackDataAsset.h"
#include "VSAttackDataAsset.h"

UVSAttackDataAsset::UVSAttackDataAsset()
{
	// NHRTodo: GAS ��� �� �ʿ��Ѱ�?
	// �ִϸ��̼� ��� �ð� ��������. 
	if (DefaultAttackMontage)
	{
		FName DefaultAttackSlotName = FName(*DefaultAttackMontageSectionName);
		DefaultAttackTime = DefaultAttackMontage->GetAnimationData(DefaultAttackSlotName)->GetLength();
	}
	if (SkillAttackMontage)
	{
		FName SkillAttackSlotName = FName(*SkillAttackMontageSectionName);
		SkillAttackTime = SkillAttackMontage->GetAnimationData(SkillAttackSlotName)->GetLength();
	}
	if (UltimateSkillAttackMontage)
	{
		FName UltimateSkillAttackSlotName = FName(*UltimateSkillAttackMontageSectionName);
		UltimateSkillAttackTime = UltimateSkillAttackMontage->GetAnimationData(UltimateSkillAttackSlotName)->GetLength();
	}
	if (DeadMontage)
	{
		FName DeadSlotName = FName(*DeadMontageSectionName);
		DeadTime = DeadMontage->GetAnimationData(DeadSlotName)->GetLength();
	}
}
