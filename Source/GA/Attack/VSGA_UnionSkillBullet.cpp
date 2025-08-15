// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_UnionSkillBullet.h"
#include "Skill/LOLVampireSurvivors/VSBulletSkill.h"
#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "ProjectLOL.h"

UVSGA_UnionSkillBullet::UVSGA_UnionSkillBullet()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_UnionSkillBullet::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
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

void UVSGA_UnionSkillBullet::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	FVector ActorLocation = AvatarActor->GetActorLocation();

	// 액터 스폰.
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UVSGA_UnionSkillBullet::SpawnAttackActor, 3.f, true, 0.2f);
}

void UVSGA_UnionSkillBullet::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_UnionSkillBullet::SpawnAttackActor()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar || !AttackActorClass) return;

	// 기존 인스턴스 제거.
	if (SpawnAttackActorInstance)
	{
		//SpawnAttackActorInstance->Destroy();
		ObjectPool->ReturnToPool(SpawnAttackActorInstance);
	}

	//FActorSpawnParameters Params;
	//Params.Owner = Avatar;
	//Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Avatar->GetActorLocation();

	AVSBaseSkill* AttackActor = Cast<AVSBaseSkill>(ObjectPool->DeferredSpawnFromPool(AttackActorClass, SpawnLocation, FRotator::ZeroRotator));
	if (AttackActor)
	{
		AttackActor->OwningActor = Avatar;
		SpawnAttackActorInstance = AttackActor;

		ObjectPool->FinishSpawn(AttackActor);
	}

	//AActor* AttaclActor = GetWorld()->SpawnActor<AActor>(AttackActorClass, SpawnLocation, FRotator::ZeroRotator, Params);
	//if (AttaclActor)
	//{
	//	//AttaclActor->AttachToActor(Avatar, FAttachmentTransformRules::KeepWorldTransform);
	//	SpawnAttackActorInstance = AttaclActor;
	//	//SpawnAttackActorInstance->SetOwner(Avatar);
	//}
}
