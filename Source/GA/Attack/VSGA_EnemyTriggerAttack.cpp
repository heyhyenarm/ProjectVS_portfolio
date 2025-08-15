// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_EnemyTriggerAttack.h"
#include "VSGA_EnemyTriggerAttack.h"

#include "Character/LOLVampireSurvivors/Enemy/VSEnemyTrashMob.h"
#include "GameData/LOLVampireSurvivors/VSAttackDataAsset.h"
#include "Attribute/VSCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/VSGameplayTag.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "ProjectLOL.h"

UVSGA_EnemyTriggerAttack::UVSGA_EnemyTriggerAttack()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_EnemyTriggerAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}

	return true;
}

void UVSGA_EnemyTriggerAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AVSEnemyTrashMob* VSEnemy = CastChecked<AVSEnemyTrashMob>(ActorInfo->AvatarActor.Get());

	UAnimMontage* AttackMontage = VSEnemy->GetAttackMontage();

	if (AttackMontage)
	{
		float AttackTime = AttackMontage->GetAnimationData(TEXT("Attack"))->GetLength();

		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage);

		// 종료 델리게이트 구독. 
		PlayAttackTask->OnCompleted.AddDynamic(this, &UVSGA_EnemyTriggerAttack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UVSGA_EnemyTriggerAttack::OnInterruptedCallback);

		// AT 구동.
		PlayAttackTask->ReadyForActivation();
	}
}

void UVSGA_EnemyTriggerAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UVSGA_EnemyTriggerAttack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UVSGA_EnemyTriggerAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
