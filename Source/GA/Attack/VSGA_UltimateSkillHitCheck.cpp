// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_UltimateSkillHitCheck.h"
#include "VSGA_SkillAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/VSAT_Trace.h"
#include "GA/TA/VSTA_Trace.h"
#include "Attribute/VSChampionAttributeSet.h"
#include "Tag/VSGameplayTag.h"

#include "ProjectLOL.h"

UVSGA_UltimateSkillHitCheck::UVSGA_UltimateSkillHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVSGA_UltimateSkillHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TA�� ���� AT ����.
	UVSAT_Trace* AttackTrackTask = UVSAT_Trace::CreateTask(this, AVSTA_Trace::StaticClass());
	// Task �Ϸ� �̺�Ʈ ����.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_UltimateSkillHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_UltimateSkillHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// Ÿ�� ������ �ڵ��� ù��° �����Ͱ� �ִ��� Ȯ��.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		//LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		//// SourceActor�� TargetActor�� ASC ��������. 
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		const UVSChampionAttributeSet* SourceAttribute = SourceASC->GetSet<UVSChampionAttributeSet>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(UltimateSkillDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetUltimateSkillPower());

			// Effect �ߵ� ���.
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	// �����Ƽ ����.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
