// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/VSTA_Trace.h"
#include "VSTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GA/AT/VSAT_Trace.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/LCCollisiion.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"

#include "AbilitySystemComponent.h"
#include "Attribute/VSChampionAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "ProjectLOL.h"
#include "Tag/VSGameplayTag.h"

AVSTA_Trace::AVSTA_Trace()
{
}

void AVSTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

}

void AVSTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AVSTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	// SourceActor의 ASC 가져오기. 
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
		return FGameplayAbilityTargetDataHandle();
	}
	// ASC의 AttributeSet 가져오기.
	const UVSChampionAttributeSet* AttributeSet = ASC->GetSet<UVSChampionAttributeSet>();
	if (!AttributeSet)
	{
		LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
		return FGameplayAbilityTargetDataHandle();
	}

	// 충돌 검출.
	TArray<FHitResult> OutHitResults;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UVSTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();

	// Attribute를 통해 스킬 별 공격 판정 값 설정하기.
	// 기본값
	float AttackRange = AttributeSet->GetAttackRange();
	float AttackRadius = AttributeSet->GetAttackRadius();

	if (ASC->HasMatchingGameplayTag(VSTAG_CHAMPION_SKILLTATTACK))
	{
		AttackRange = AttributeSet->GetSkillRange();
		AttackRadius = AttributeSet->GetSkillRadius();
	}
	else if (ASC->HasMatchingGameplayTag(VSTAG_CHAMPION_ULTIMATESKILLTATTACK))
	{
		AttackRange = AttributeSet->GetUltimateSkillRange();
		AttackRadius = AttributeSet->GetUltimateSkillRadius();
	}

	// 충돌 검출 시작 지점.
	FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	// 충돌 검출 종료 지점.
	FVector End = Start + Forward * AttackRange;

	// 충돌 판정.
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, TCC_CHARACTER, FCollisionShape::MakeSphere(AttackRadius), Params);

	// 충돌한 타겟 데이터들 검출.
	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		for (const FHitResult& Hit : OutHitResults)
		{
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
			DataHandle.Add(TargetData);
		}
	}

//#if ENABLE_DRAW_DEBUG
//
//	if (bShowDebug)
//	{
//		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//		DrawDebugSphere(
//			GetWorld(),
//			Start + (End - Start) * 0.5f,
//			AttackRadius,
//			12,
//			DrawColor,
//			false,
//			0.5f);
//	}
//#endif

	return DataHandle;
}
