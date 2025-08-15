// Fill out your copyright notice in the Description page of Project Settings.

#include "VSTA_EnemyTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GA/AT/VSAT_EnemyTrace.h"

#include "GameFramework/Character.h"
#include "Character/LOLVampireSurvivors/Enemy/VSEnemyTrashMob.h"
#include "Components/SphereComponent.h"
#include "Physics/LCCollisiion.h"
#include "CollisionShape.h"
#include "DrawDebugHelpers.h"

#include "AbilitySystemComponent.h"
#include "Attribute/VSCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "ProjectLOL.h"

AVSTA_EnemyTrace::AVSTA_EnemyTrace()
{
}

void AVSTA_EnemyTrace::StartTargeting(UGameplayAbility* Ability)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

}

void AVSTA_EnemyTrace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AVSTA_EnemyTrace::MakeTargetData() const
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	AVSEnemyTrashMob* Character = CastChecked<AVSEnemyTrashMob>(SourceActor);

	// SourceActor�� ASC ��������. 
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
		return FGameplayAbilityTargetDataHandle();
	}
	// ASC�� AttributeSet ��������.
	const UVSCharacterAttributeSet* AttributeSet = ASC->GetSet<UVSCharacterAttributeSet>();
	if (!AttributeSet)
	{
		LOL_LOG(LogProjectLOL, Error, TEXT("ASC NOT FOUND"));
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult OutHitResult;
	// ���� ���� ����.
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius() * 2.f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UVSTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	// �浹 ���� ���� ����.
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetSphereComponent()->GetScaledSphereRadius();
	// �浹 ���� ���� ����.
	const FVector End = Start + Forward * AttackRange;

	// �浹 ����.
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, TCC_CHARACTER, FCollisionShape::MakeSphere(AttackRadius), Params);

	// �浹�� Ÿ�� ������ ����.
	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

//#if ENABLE_DRAW_DEBUG
//
//
//	if (bShowDebug)
//	{
//		DrawDebugSphere(
//			GetWorld(),
//			End, 
//			AttackRadius, 
//			16, 
//			HitDetected ? FColor::Green : FColor::Red, 
//			false, 
//			2.0f);
//	}
//#endif

	return DataHandle;
}
