// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "VSAT_SkillAttack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillAttackDelegate);

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSAT_SkillAttack : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
	
public:
	explicit UVSAT_SkillAttack();

	// AT 인스턴스 생성 반환 함수.
	static UVSAT_SkillAttack* CreateTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	FSkillAttackDelegate OnCompleted;
};
