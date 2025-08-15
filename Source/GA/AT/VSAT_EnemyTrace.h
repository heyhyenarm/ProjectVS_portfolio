// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "VSAT_EnemyTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSAT_EnemyTrace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	explicit UVSAT_EnemyTrace();

	// AT �ν��Ͻ� ���� ��ȯ �Լ�.
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UVSAT_EnemyTrace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AVSTA_EnemyTrace> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	// Ÿ�� ���� ����. 
	void SpawnAndInitializeTargetActor();
	// Ÿ�� ���� Ȯ��.
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandel);

public:
	UPROPERTY(BlueprintAssignable)
	FEnemyTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AVSTA_EnemyTrace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AVSTA_EnemyTrace> SpawnedTargetActor;

};
