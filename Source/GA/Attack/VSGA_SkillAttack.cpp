// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_SkillAttack.h"
#include "Character/LOLVampireSurvivors/VSCharacterBase.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameData/LOLVampireSurvivors/VSAttackDataAsset.h"
#include "ProjectLOL.h"

UVSGA_SkillAttack::UVSGA_SkillAttack()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bCanAttack = true;
}

void UVSGA_SkillAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bCanAttack = false;
	AVSCharacterChampionPlayer* VSCharacterChampion = CastChecked<AVSCharacterChampionPlayer>(ActorInfo->AvatarActor.Get());

	UVSAttackDataAsset* AttackData = VSCharacterChampion->GetAttackData();
	UAnimMontage* SkillAttackMontage = AttackData->SkillAttackMontage;
	if (SkillAttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlaySkillAttack"), SkillAttackMontage);

		// 종료 델리게이트 구독. 
		PlayAttackTask->OnCompleted.AddDynamic(this, &UVSGA_SkillAttack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UVSGA_SkillAttack::OnInterruptedCallback);

		// AT 구동.
		PlayAttackTask->ReadyForActivation();
	
		// 스킬 타이머 설정. 
		GetWorld()->GetTimerManager().SetTimer(
			SkillAttackTimerHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					//LOL_LOG(LogProjectLOL, Log, TEXT("SkillAttackEnd"));
					bCanAttack = true;
				}), AttackData->SkillAttackTime + AttackData->SkillAttackCoolTime, false
		);
	}
}

void UVSGA_SkillAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_SkillAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

//bool UVSGA_SkillAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
//{
//	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
//	if (!bResult)
//	{
//		return false;
//	}
//
//	return bCanAttack;
//}
