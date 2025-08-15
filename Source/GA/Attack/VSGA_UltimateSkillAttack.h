// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA/Attack/VSGA_AttackBase.h"
#include "VSGA_UltimateSkillAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSGA_UltimateSkillAttack : public UVSGA_AttackBase
{
	GENERATED_BODY()
	
public:
	explicit UVSGA_UltimateSkillAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	FTimerHandle UltimateSkillAttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UNiagaraSystem* Niagara;

	class UNiagaraComponent* NiagaraInstance;
};
