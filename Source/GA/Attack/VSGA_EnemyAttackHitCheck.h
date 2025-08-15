// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VSGA_EnemyAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSGA_EnemyAttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	explicit UVSGA_EnemyAttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// Trace AT ���� �� �ݹ�.
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;


};
