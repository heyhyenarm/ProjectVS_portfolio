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

	// TA�� ���� AT ����.
	UVSAT_Trace* AttackTrackTask = UVSAT_Trace::CreateTask(this, AVSTA_Trace::StaticClass());
	// Task �Ϸ� �̺�Ʈ ����.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_DefaultAttackHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_DefaultAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// Ÿ�� ������ �ڵ��� ù��° �����Ͱ� �ִ��� Ȯ��.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		for (int32 i = 0; i < TargetDataHandle.Num(); ++i)
		{
			const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(i);

			FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, i);
			LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

			// SourceActor�� ASC ��������. 
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

			const UVSChampionAttributeSet* SourceAttribute = SourceASC->GetSet<UVSChampionAttributeSet>();

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
			if (EffectSpecHandle.IsValid())
			{
				// Tag���� �ٸ� Attribute Damage ����.
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

				// Effect �ߵ� ���.
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			}
		}
	}

	// �����Ƽ ����.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
