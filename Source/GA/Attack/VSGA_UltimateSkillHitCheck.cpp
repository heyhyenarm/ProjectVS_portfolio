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

	// TA를 가진 AT 생성.
	UVSAT_Trace* AttackTrackTask = UVSAT_Trace::CreateTask(this, AVSTA_Trace::StaticClass());
	// Task 완료 이벤트 구독.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_UltimateSkillHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_UltimateSkillHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 타겟 데이터 핸들의 첫번째 데이터가 있는지 확인.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		//LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		//// SourceActor와 TargetActor의 ASC 가져오기. 
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		const UVSChampionAttributeSet* SourceAttribute = SourceASC->GetSet<UVSChampionAttributeSet>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(UltimateSkillDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetUltimateSkillPower());

			// Effect 발동 명령.
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	// 어빌리티 종료.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
