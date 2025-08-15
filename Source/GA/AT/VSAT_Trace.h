// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "VSAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	explicit UVSAT_Trace();

	// AT �ν��Ͻ� ���� ��ȯ �Լ�.
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UVSAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AVSTA_Trace> TargetActorClass);

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
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AVSTA_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AVSTA_Trace> SpawnedTargetActor;

};
