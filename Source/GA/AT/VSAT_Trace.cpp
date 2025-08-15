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
	// �½�ũ�� ������ �غ�.
	UVSAT_Trace* NewTask = NewAbilityTask<UVSAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = InTargetActorClass;
	return NewTask;
}

void UVSAT_Trace::Activate()
{
	Super::Activate();

	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	// Ÿ�� ���� ����.
	SpawnAndInitializeTargetActor();

	// Ÿ�� ���� Ȯ��.
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UVSAT_Trace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	// Task ���� �� TargetActor�� �ִٸ�, �����ϱ�.
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
		// ��������Ʈ�� �Լ� ���. 
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UVSAT_Trace::OnTargetDataReadyCallback);
	}
}

void UVSAT_Trace::FinalizeTargetActor()
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

void UVSAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
