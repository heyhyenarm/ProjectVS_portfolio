// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VSGA_SkillAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSGA_SkillAttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	explicit UVSGA_SkillAttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// Trace AT 종료 시 콜백.
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> SkillDamageEffect;
};
