// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LOLVampireSurvivors/Champion/VSAnimNotify_SkillHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UVSAnimNotify_SkillHitCheck::UVSAnimNotify_SkillHitCheck()
{
}

FString UVSAnimNotify_SkillHitCheck::GetNotifyName_Implementation() const
{
	return FString(TEXT("VSSkillHitCheck"));
}

void UVSAnimNotify_SkillHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
