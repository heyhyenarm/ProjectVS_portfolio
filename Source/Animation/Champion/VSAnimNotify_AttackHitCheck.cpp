// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LOLVampireSurvivors/Champion/VSAnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UVSAnimNotify_AttackHitCheck::UVSAnimNotify_AttackHitCheck()
{
}

FString UVSAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	// 에디터에 표시 할 텍스트.
	return TEXT("VSAttackHitCheck");
}

void UVSAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 캐릭터인지 확인.
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
