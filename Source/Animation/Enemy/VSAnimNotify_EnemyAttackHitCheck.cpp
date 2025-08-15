// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LOLVampireSurvivors/Enemy/VSAnimNotify_EnemyAttackHitCheck.h"
#include "VSAnimNotify_EnemyAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UVSAnimNotify_EnemyAttackHitCheck::UVSAnimNotify_EnemyAttackHitCheck()
{
}

FString UVSAnimNotify_EnemyAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("VSEnemyAttackHitCheck");
}

void UVSAnimNotify_EnemyAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			// ASC를 가진 지정한 액터에 지정한 태그를 넣어 이벤트 발생시키기 위한 기능.
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
