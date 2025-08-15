// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_EnemyAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/VSAT_EnemyTrace.h"
#include "GA/TA/VSTA_EnemyTrace.h"
#include "Attribute/VSCharacterAttributeSet.h"
#include "Tag/VSGameplayTag.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"

#include "ProjectLOL.h"

UVSGA_EnemyAttackHitCheck::UVSGA_EnemyAttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVSGA_EnemyAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TA�� ���� AT ����.
	UVSAT_EnemyTrace* AttackTrackTask = UVSAT_EnemyTrace::CreateTask(this, AVSTA_EnemyTrace::StaticClass());
	// Task �Ϸ� �̺�Ʈ ����.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_EnemyAttackHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_EnemyAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	// Ÿ�� ������ �ڵ��� ù��° �����Ͱ� �ִ��� Ȯ��.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		//LOL_LOG(LogProjectLOL, Warning, TEXT("HitResult Actor : %s"), *HitResult.GetActor()->GetName());

		if (IsValid(Cast<AVSCharacterChampionPlayer>(HitResult.GetActor())))
		{
			//LOL_LOG(LogProjectLOL, Warning, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

			// SourceActor�� ASC ��������. 
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

			const UVSCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UVSCharacterAttributeSet>();

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetAttackPower());

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
