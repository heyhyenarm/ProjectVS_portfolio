// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/Attack/VSGA_AttackBase.h"
#include "Interface/LOLClassic/LCPoolInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "VSGA_UnionSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSGA_UnionSkill : public UVSGA_AttackBase, public ILCPoolInterface
{
	GENERATED_BODY()

public:
	explicit UVSGA_UnionSkill();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual void SpawnAttackActor(FVector Location);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AttackActorClass;

	class AVSBaseSkill* SpawnAttackActorInstance;

	UPROPERTY(EditAnywhere)
	FTimerHandle AttackHandle;

	UPROPERTY(EditAnywhere)
	float AttackRadius;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffect;

public:
	UPROPERTY()
	TObjectPtr<class ULCObjectPoolingSubsystem> ObjectPool;
};
