// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/Attack/VSGA_UnionSkill.h"
#include "VSGA_UnionSkillBounce.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSGA_UnionSkillBounce : public UVSGA_UnionSkill
{
	GENERATED_BODY()

public:
	explicit UVSGA_UnionSkillBounce();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual void SpawnAttackActor(FVector Location) override;

public:
	void OnTick();

protected:
	FTimerHandle TickTimerHandle;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<class UNiagaraSystem> NiagaraSystem;
};
