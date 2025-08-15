// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/Attack/VSGA_UnionSkill.h"
#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"

UVSGA_UnionSkill::UVSGA_UnionSkill()
{
	// GA의 인스턴싱 정책 지정.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_UnionSkill::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	//bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	//if (!bResult)
	//{
	//	return false;
	//}

	//const AVSCharacterChampionPlayer* ChampionPlayer = Cast<AVSCharacterChampionPlayer>(ActorInfo->AvatarActor.Get());

	//return ChampionPlayer->bAttackDirty;

	return false;
}

void UVSGA_UnionSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ObjectPool = GetWorld()->GetSubsystem<ULCObjectPoolingSubsystem>();
}

void UVSGA_UnionSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UVSGA_UnionSkill::SpawnAttackActor(FVector Location)
{
}
