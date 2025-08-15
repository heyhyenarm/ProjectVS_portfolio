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

	// TA를 가진 AT 생성.
	UVSAT_EnemyTrace* AttackTrackTask = UVSAT_EnemyTrace::CreateTask(this, AVSTA_EnemyTrace::StaticClass());
	// Task 완료 이벤트 구독.
	AttackTrackTask->OnComplete.AddDynamic(this, &UVSGA_EnemyAttackHitCheck::OnTraceResultCallback);
	AttackTrackTask->ReadyForActivation();
}

void UVSGA_EnemyAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	// 타겟 데이터 핸들의 첫번째 데이터가 있는지 확인.
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		//LOL_LOG(LogProjectLOL, Warning, TEXT("HitResult Actor : %s"), *HitResult.GetActor()->GetName());

		if (IsValid(Cast<AVSCharacterChampionPlayer>(HitResult.GetActor())))
		{
			//LOL_LOG(LogProjectLOL, Warning, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

			// SourceActor의 ASC 가져오기. 
			UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

			const UVSCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UVSCharacterAttributeSet>();

			FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, -SourceAttribute->GetAttackPower());

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
