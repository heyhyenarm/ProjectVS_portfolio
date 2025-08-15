// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_UltimateSkillAttack.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameData/LOLVampireSurvivors/VSAttackDataAsset.h"
#include "ProjectLOL.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UVSGA_UltimateSkillAttack::UVSGA_UltimateSkillAttack()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bCanAttack = true;
}

void UVSGA_UltimateSkillAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bCanAttack = false;

	AVSCharacterChampionPlayer* VSCharacterChampion = CastChecked<AVSCharacterChampionPlayer>(ActorInfo->AvatarActor.Get());

	UVSAttackDataAsset* AttackData = VSCharacterChampion->GetAttackData();
	UAnimMontage* UltimateSkillAttackMontage = AttackData->UltimateSkillAttackMontage;
	if (UltimateSkillAttackMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayUltimateSkillAttack"), UltimateSkillAttackMontage);

		// 종료 델리게이트 구독. 
		PlayAttackTask->OnCompleted.AddDynamic(this, &UVSGA_UltimateSkillAttack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UVSGA_UltimateSkillAttack::OnInterruptedCallback);

		// AT 구동.
		PlayAttackTask->ReadyForActivation();

		if (Niagara)
		{
			FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
			FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();

			// 나이아가라 이펙트 스폰.
			// @NHRTodo: 오브젝트 풀링.
			NiagaraInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				ActorInfo->AvatarActor->GetWorld(),
				Niagara,
				SpawnLocation,
				SpawnRotation,
				FVector(5.f)
			);

			LOL_LOG(LogProjectLOL, Log, TEXT("NiagaraInstance %s"), *NiagaraInstance->GetName());
		}

		GetWorld()->GetTimerManager().SetTimer(
			UltimateSkillAttackTimerHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					//LOL_LOG(LogProjectLOL, Log, TEXT("UltimateSkillAttackEnd"));
					bCanAttack = true;
				}), AttackData->UltimateSkillAttackTime + AttackData->UltimateSkillAttackCoolTime, false
		);
	}
}

void UVSGA_UltimateSkillAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_UltimateSkillAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (NiagaraInstance)
	{
		LOL_LOG(LogProjectLOL, Log, TEXT("NiagaraInstance Deactivate"));
		NiagaraInstance->Deactivate();
	}
}
