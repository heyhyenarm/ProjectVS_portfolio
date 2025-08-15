// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_SkillAttackHitCheck.h"
#include "VSGA_SkillAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/VSAT_Trace.h"
#include "GA/TA/VSTA_Trace.h"
#include "Attribute/VSChampionAttributeSet.h"
#include "Tag/VSGameplayTag.h"

#include "ProjectLOL.h"

UVSGA_SkillAttackHitCheck::UVSGA_SkillAttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVSGA_SkillAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TA�� ���� AT ����.
	UVSAT_Trace* AttackTrackTask = UVSAT_Trace::CreateTask(this, AVSTA_Trace::StaticClass());
	// Task �Ϸ� �̺�Ʈ ����.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_SkillAttackHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_SkillAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// Ÿ�� ������ �ڵ��� ù��° �����Ͱ� �ִ��� Ȯ��.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		//LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		//// SourceActor�� TargetActor�� ASC ��������. 
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		//UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		//if (!SourceASC || !TargetASC)
		//{
		//	LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
		//	return;
		//}

		const UVSChampionAttributeSet* SourceAttribute = SourceASC->GetSet<UVSChampionAttributeSet>();
		//// TargetActor�� �������� �ֱ� ���� �� ���� �ʿ� -> �ӽ������� const_cast�� �Ӽ��� �����Ͽ� �� ���� ��. 
		//// ���� GameplayEffect�� ����Ͽ� ������ ��. 
		//UVSCharacterAttributeSet* TargetAttribute = const_cast<UVSCharacterAttributeSet*>(TargetASC->GetSet<UVSCharacterAttributeSet>());
		//if (!SourceAttribute || !TargetAttribute)
		//{
		//	LOL_LOG(LogProjectLOL, Error, TEXT("Attribute NOT FOUND"));
		//	return;
		//}
		//
		//// ������ �ֱ�.
		//const float AttackDamage = SourceAttribute->GetAttackPower();
		//TargetAttribute->SetHP(TargetAttribute->GetHP() - AttackDamage);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(SkillDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetSkillPower());

			//LOL_LOG(LogProjectLOL, Log, TEXT("Skill Power : %f"), -SourceAttribute->GetSkillPower());

			// Effect �ߵ� ���.
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	// �����Ƽ ����.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
