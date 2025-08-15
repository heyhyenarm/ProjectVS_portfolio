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
	// �½�ũ�� ������ �غ�.
	UVSAT_EnemyTrace* NewTask = NewAbilityTask<UVSAT_EnemyTrace>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;
	return NewTask;
}

void UVSAT_EnemyTrace::Activate()
{
	Super::Activate();

	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	// Ÿ�� ���� ����.
	SpawnAndInitializeTargetActor();

	// Ÿ�� ���� Ȯ��.
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UVSAT_EnemyTrace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	// Task ���� �� TargetActor�� �ִٸ�, �����ϱ�.
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
		// ��������Ʈ�� �Լ� ���. 
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UVSAT_EnemyTrace::OnTargetDataReadyCallback);
	}
}

void UVSAT_EnemyTrace::FinalizeTargetActor()
{
	// ���� Ÿ�� ���� ����.
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Add(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);

		// Ÿ���� Ȯ��.
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
