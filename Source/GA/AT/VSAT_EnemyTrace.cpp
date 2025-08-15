// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/VSAT_EnemyTrace.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GA/TA/VSTA_EnemyTrace.h"
#include "AbilitySystemComponent.h"

#include "ProjectLOL.h"

UVSAT_EnemyTrace::UVSAT_EnemyTrace()
{
}

UVSAT_EnemyTrace* UVSAT_EnemyTrace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AVSTA_EnemyTrace> InTargetActorClass)
{
	// 태스크의 생성과 준비.
	UVSAT_EnemyTrace* NewTask = NewAbilityTask<UVSAT_EnemyTrace>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;
	return NewTask;
}

void UVSAT_EnemyTrace::Activate()
{
	Super::Activate();

	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	// 타겟 액터 생성.
	SpawnAndInitializeTargetActor();

	// 타겟 액터 확정.
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UVSAT_EnemyTrace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	// Task 종료 시 TargetActor가 있다면, 삭제하기.
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
}

void UVSAT_EnemyTrace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AVSTA_EnemyTrace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		// 델리게이트에 함수 등록. 
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UVSAT_EnemyTrace::OnTargetDataReadyCallback);
	}
}

void UVSAT_EnemyTrace::FinalizeTargetActor()
{
	// 최종 타겟 액터 검증.
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Add(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);

		// 타겟팅 확정.
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UVSAT_EnemyTrace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
