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

	// TA를 가진 AT 생성.
	UVSAT_Trace* AttackTrackTask = UVSAT_Trace::CreateTask(this, AVSTA_Trace::StaticClass());
	// Task 완료 이벤트 구독.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_SkillAttackHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_SkillAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 타겟 데이터 핸들의 첫번째 데이터가 있는지 확인.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		//LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		//// SourceActor와 TargetActor의 ASC 가져오기. 
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		//UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		//if (!SourceASC || !TargetASC)
		//{
		//	LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
		//	return;
		//}

		const UVSChampionAttributeSet* SourceAttribute = SourceASC->GetSet<UVSChampionAttributeSet>();
		//// TargetActor에 데미지를 주기 위해 값 변경 필요 -> 임시적으로 const_cast로 속성을 변경하여 값 변경 중. 
		//// 이후 GameplayEffect를 사용하여 변경할 것. 
		//UVSCharacterAttributeSet* TargetAttribute = const_cast<UVSCharacterAttributeSet*>(TargetASC->GetSet<UVSCharacterAttributeSet>());
		//if (!SourceAttribute || !TargetAttribute)
		//{
		//	LOL_LOG(LogProjectLOL, Error, TEXT("Attribute NOT FOUND"));
		//	return;
		//}
		//
		//// 데미지 주기.
		//const float AttackDamage = SourceAttribute->GetAttackPower();
		//TargetAttribute->SetHP(TargetAttribute->GetHP() - AttackDamage);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(SkillDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetSkillPower());

			//LOL_LOG(LogProjectLOL, Log, TEXT("Skill Power : %f"), -SourceAttribute->GetSkillPower());

			// Effect 발동 명령.
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	// 어빌리티 종료.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
