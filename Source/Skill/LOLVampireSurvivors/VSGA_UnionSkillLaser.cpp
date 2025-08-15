// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSGA_UnionSkillLaser.h"
#include "Skill/LOLVampireSurvivors/VSLaserSkill.h"
#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "ProjectLOL.h"


UVSGA_UnionSkillLaser::UVSGA_UnionSkillLaser()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AttackRadius = 500.f;
}

bool UVSGA_UnionSkillLaser::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void UVSGA_UnionSkillLaser::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	FVector ActorLocation = AvatarActor->GetActorLocation();

	// 나이아가라 스폰.
	//UNiagaraComponent* NiagaraComp1 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(AvatarActor, AttackNiagara, ActorLocation + (FVector::ForwardVector * AttackRadius));
	//UNiagaraComponent* NiagaraComp2 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(AvatarActor, AttackNiagara, ActorLocation - (FVector::ForwardVector * AttackRadius));
	//UNiagaraComponent* NiagaraComp3 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(AvatarActor, AttackNiagara, ActorLocation + (FVector::RightVector * AttackRadius));
	//UNiagaraComponent* NiagaraComp4 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(AvatarActor, AttackNiagara, ActorLocation - (FVector::RightVector * AttackRadius));

	// 액터 스폰.
	SpawnAttackActor(ActorLocation);

	//GetWorld()->GetTimerManager().SetTimer(
	//	AttackHandle,
	//	FTimerDelegate::CreateLambda([&]()
	//		{
	//			SpawnAttackActor(ActorLocation);
	//			Cast<AVSLaserSkill>(SpawnAttackActorInstance)->Damage();
	//		}),
	//	7.f,
	//	true
	//);

	// AttackHitCheck. 

}

void UVSGA_UnionSkillLaser::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_UnionSkillLaser::SpawnAttackActor(FVector Location)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar || !AttackActorClass) return;

	// 기존 인스턴스 제거.
	if (SpawnAttackActorInstance)
	{
		ObjectPool->ReturnToPool(SpawnAttackActorInstance);
	}

	//FActorSpawnParameters Params;
	//Params.Owner = Avatar;
	//Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AVSBaseSkill* AttackActor = Cast<AVSBaseSkill>(ObjectPool->DeferredSpawnFromPool(AttackActorClass, Location, FRotator::ZeroRotator));
	if (AttackActor)
	{
		AttackActor->OwningActor = Avatar;
		SpawnAttackActorInstance = AttackActor;

		ObjectPool->FinishSpawn(AttackActor);
	}

	//AVSBaseSkill* AttaclActor = GetWorld()->SpawnActor<AVSBaseSkill>(AttackActorClass, Location, FRotator::ZeroRotator, Params);
	//if (AttaclActor)
	//{
	//	//AttaclActor->AttachToActor(Avatar, FAttachmentTransformRules::KeepWorldTransform);
	//	AttaclActor->OwningActor = Avatar;
	//	SpawnAttackActorInstance = AttaclActor;
	//	//SpawnAttackActorInstance->SetOwner(Avatar);
	//}
}

