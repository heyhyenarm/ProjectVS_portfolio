// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSGA_UnionSkillFireBall.h"
#include "Skill/LOLVampireSurvivors/VSFireBallSkill.h"
#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "ProjectLOL.h"

UVSGA_UnionSkillFireBall::UVSGA_UnionSkillFireBall()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_UnionSkillFireBall::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void UVSGA_UnionSkillFireBall::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	FVector ActorLocation = AvatarActor->GetActorLocation();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UVSGA_UnionSkillFireBall::SpawnAttackActor, 3.f, true, 0.2f);

	//SpawnAttackActor();
}

void UVSGA_UnionSkillFireBall::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_UnionSkillFireBall::SpawnAttackActor()
{
	//LOL_LOG(LogProjectLOL, Warning, TEXT("SpawnAttackActor"));

	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar || !AttackActorClass) return;

	for (auto ActorInstance : SpawnAttackActorInstances)
	{
		// 기존 인스턴스 제거
		if (ActorInstance)
		{
			//ActorInstance->Destroy();
			ObjectPool->ReturnToPool(SpawnAttackActorInstance);
		}

		//FActorSpawnParameters Params;
		//Params.Owner = Avatar;
		//Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector2D RandomOffset = FMath::RandPointInCircle(1000.f);
		FVector SpawnLocation = Avatar->GetActorLocation() + FVector(RandomOffset.X - 1000.f, RandomOffset.Y - 1000.f, 800.f);

		AVSBaseSkill* AttackActor = Cast<AVSBaseSkill>(ObjectPool->DeferredSpawnFromPool(AttackActorClass, SpawnLocation, FRotator::ZeroRotator));
		if (AttackActor)
		{
			AttackActor->OwningActor = Avatar;
			ActorInstance = AttackActor;

			ObjectPool->FinishSpawn(AttackActor);
		}

		//AActor* AttackActor = GetWorld()->SpawnActor<AActor>(AttackActorClass, SpawnLocation, FRotator::ZeroRotator, Params);
		//if (AttackActor)
		//{
		//	//AttackActor->AttachToActor(Avatar, FAttachmentTransformRules::KeepWorldTransform);
		//	ActorInstance = AttackActor;
		//	//ActorInstance->SetOwner(Avatar);
		//}
	}
}
