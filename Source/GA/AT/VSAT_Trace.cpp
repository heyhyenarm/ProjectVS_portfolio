// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/VSAT_Trace.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GA/TA/VSTA_Trace.h"
#include "AbilitySystemComponent.h"

#include "ProjectLOL.h"

UVSAT_Trace::UVSAT_Trace()
{
}

UVSAT_Trace* UVSAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AVSTA_Trace> InTargetActorClass)
{
	// 태스크의 생성과 준비.
	UVSAT_Trace* NewTask = NewAbilityTask<UVSAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;
	return NewTask;
}

void UVSAT_Trace::Activate()
{
	Super::Activate();

	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	// 타겟 액터 생성.
	SpawnAndInitializeTargetActor();

	// 타겟 액터 확정.
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UVSAT_Trace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	// Task 종료 시 TargetActor가 있다면, 삭제하기.
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
}

void UVSAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AVSTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		// 델리게이트에 함수 등록. 
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UVSAT_Trace::OnTargetDataReadyCallback);
	}
}

void UVSAT_Trace::FinalizeTargetActor()
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

void UVSAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
