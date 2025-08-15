// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "VSTA_EnemyTrace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API AVSTA_EnemyTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	explicit AVSTA_EnemyTrace();

	// 타겟팅 시작. 
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	// 타겟팅 확정 후 AT 발동.
	virtual void ConfirmTargetingAndContinue() override;

	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;

};
