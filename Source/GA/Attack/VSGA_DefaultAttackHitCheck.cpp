// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_DefaultAttackHitCheck.h"
#include "VSGA_SkillAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/VSAT_Trace.h"
#include "GA/TA/VSTA_Trace.h"
#include "Attribute/VSChampionAttributeSet.h"
#include "Tag/VSGameplayTag.h"

#include "ProjectLOL.h"

UVSGA_DefaultAttackHitCheck::UVSGA_DefaultAttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVSGA_DefaultAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TA를 가진 AT 생성.
	UVSAT_Trace* AttackTrackTask = UVSAT_Trace::CreateTask(this, AVSTA_Trace::StaticClass());
	// Task 완료 이벤트 구독.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_DefaultAttackHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_DefaultAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 타겟 데이터 핸들의 첫번째 데이터가 있는지 확인.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		for (int32 i = 0; i < TargetDataHandle.Num(); ++i)
		{
			const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(i);

			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, i);
			LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

			// SourceActor의 ASC 가져오기. 
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

			const UVSChampionAttributeSet* SourceAttribute = SourceASC->GetSet<UVSChampionAttributeSet>();

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
			if (EffectSpecHandle.IsValid())
			{
				// Tag마다 다른 Attribute Damage 설정.
				EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetAttackPower());

				UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
				if (!ASC)
				{
					LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
					EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetAttackPower());
				}
				else
				{
					if (ASC->HasMatchingGameplayTag(VSTAG_CHAMPION_SKILLTATTACK))
					{
						EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetSkillPower());
					}
					else if (ASC->HasMatchingGameplayTag(VSTAG_CHAMPION_ULTIMATESKILLTATTACK))
					{
						EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetUltimateSkillPower());
					}
					else
					{
						EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetAttackPower());
					}
				}

				//LOL_LOG(LogProjectLOL, Log, TEXT("Attack Power : %f"), -SourceAttribute->GetAttackPower());

				// Effect 발동 명령.
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			}
		}
	}

	// 어빌리티 종료.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
