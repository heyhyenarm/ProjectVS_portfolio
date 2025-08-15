// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LOLVampireSurvivors/Champion/VSAnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UVSAnimNotify_AttackHitCheck::UVSAnimNotify_AttackHitCheck()
{
}

FString UVSAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	// �����Ϳ� ǥ�� �� �ؽ�Ʈ.
	return TEXT("VSAttackHitCheck");
}

void UVSAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// ĳ�������� Ȯ��.
	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			// ASC�� ���� ������ ���Ϳ� ������ �±׸� �־� �̺�Ʈ �߻���Ű�� ���� ���.
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
