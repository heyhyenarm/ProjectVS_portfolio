// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LOLVampireSurvivors/Champion/VSAnimNotify_UltimateHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UVSAnimNotify_UltimateHitCheck::UVSAnimNotify_UltimateHitCheck()
{
}

FString UVSAnimNotify_UltimateHitCheck::GetNotifyName_Implementation() const
{
	return FString(TEXT("VSUltimateSkillHitCheck"));
}

void UVSAnimNotify_UltimateHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
