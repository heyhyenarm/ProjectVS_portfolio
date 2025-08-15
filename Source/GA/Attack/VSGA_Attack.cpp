// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_Attack.h"
#include "Character/LOLVampireSurvivors/VSCharacterBase.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"
#include "GameData/LOLVampireSurvivors/VSAttackDataAsset.h"
#include "GameFramework/SpringArmComponent.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

#include "SubSystem/LOLVampireSurvivors/BoidManager.h"
#include "ProjectLOL.h"

UVSGA_Attack::UVSGA_Attack()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	//bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	//if (!bResult)
	//{
	//	return false;
	//}

	//const AVSCharacterChampionPlayer* ChampionPlayer = Cast<AVSCharacterChampionPlayer>(ActorInfo->AvatarActor.Get());

	//return ChampionPlayer->bAttackDirty;

	// NHTTodo : 강제 실행 중.
	return true;
}

void UVSGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AVSCharacterChampionPlayer* VSCharacterChampion = CastChecked<AVSCharacterChampionPlayer>(ActorInfo->AvatarActor.Get());
	
	// 가장 가까운 Boid를 향해 회전.
	AActor* TargetBoid = GetWorld()->GetSubsystem<UBoidManager>()->GetAutoTargetBoid();
	if (TargetBoid)
	{
		FVector TargetLocation = TargetBoid->GetActorLocation();

		FVector Direction = TargetLocation - VSCharacterChampion->GetActorLocation();
		Direction.Z = 0;

		FRotator ToTargetRotation = Direction.Rotation();
		FRotator CurrentRotation = VSCharacterChampion->GetActorRotation();

		ToTargetRotation.Pitch = CurrentRotation.Pitch;
		ToTargetRotation.Roll = CurrentRotation.Roll;

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, ToTargetRotation, GetWorld()->GetDeltaSeconds(), 50.0f);

		VSCharacterChampion->SetActorRelativeRotation(NewRotation);

		//if (VSCharacterChampion->GetSpringArm())
		//{
		//	FRotator FixedSpringArmRot = FRotator(0.f, 0.f, 0.f);
		//	VSCharacterChampion->GetSpringArm()->SetWorldRotation(FixedSpringArmRot);
		//	//FixedSpringArmRot = FRotator(180.f, -125.f, 180.f);
		//	//VSCharacterChampion->GetSpringArm()->SetRelativeRotation(FixedSpringArmRot);
		//}
	}

	UVSAttackDataAsset* AttackData = VSCharacterChampion->GetAttackData();
	UAnimMontage* DefaultAttackMontage = AttackData->DefaultAttackMontage;

	if (DefaultAttackMontage)
	{
		float DefaultAttackTime = DefaultAttackMontage->GetAnimationData(TEXT("DefaultAttack"))->GetLength();

		//UAbilityTask_WaitDelay* DelayAT = UAbilityTask_WaitDelay::WaitDelay(this, 3.f);
		//DelayAT->OnFinish.AddDynamic(this, &UVSGA_Attack::StartLoopAttack);
		//DelayAT->ReadyForActivation();

		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDefaultAttack"), DefaultAttackMontage);
	
		// 종료 델리게이트 구독. 
		PlayAttackTask->OnCompleted.AddDynamic(this, &UVSGA_Attack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UVSGA_Attack::OnInterruptedCallback);

		// AT 구동.
		PlayAttackTask->ReadyForActivation();

		//FTimerHandle DefaultAttackTimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(
		//	DefaultAttackTimerHandle,
		//	FTimerDelegate::CreateLambda([&]()
		//		{
		//			//LOL_LOG(LogProjectLOL, Log, TEXT("AttackEnd"));
		//			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		//		}), AttackData->DefaultAttackTime + AttackData->DefaultAttackCoolTime, true
		//);
	}
}

void UVSGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_Attack::StartLoopAttack(UAnimMontage* DefaultAttackMontage)
{
}
