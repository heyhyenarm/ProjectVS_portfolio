// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/VSGA_GiveExp.h"
#include "Tag/VSGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "Attribute/VSExpAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "ProjectLOL.h"

UVSGA_GiveExp::UVSGA_GiveExp()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_GiveExp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

    //LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
    return true;
}

void UVSGA_GiveExp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    //LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
    
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

	const UVSExpAttributeSet* SourceAttribute = SourceASC->GetSet<UVSExpAttributeSet>();

	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GainExpEffect);
	if (EffectSpecHandle.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_CHAMPION_GAINEXP, SourceAttribute->GetExpAmount());

		//LOL_LOG(LogProjectLOL, Log, TEXT("Exp Amount : %f"), SourceAttribute->GetExpAmount());

		// Effect 발동 명령.
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);

		// 어빌리티 종료.
		bool bReplicatedEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
	}

}

//void UVSGA_GiveExp::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
//{
//	// 타겟 데이터 핸들의 첫번째 데이터가 있는지 확인.
//	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
//	{
//		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
//		LOL_LOG(LogProjectLOL, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
//
//		// SourceActor의 ASC 가져오기. 
//		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
//
//		const UVSExpAttributeSet* SourceAttribute = SourceASC->GetSet<UVSExpAttributeSet>();
//
//		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(GainExpEffect);
//		if (EffectSpecHandle.IsValid())
//		{
//			EffectSpecHandle.Data->SetSetByCallerMagnitude(VSTAG_DATA_DAMAGE, SourceAttribute->GetExpAmount());
//
//			LOL_LOG(LogProjectLOL, Log, TEXT("Exp Amount : %f"), SourceAttribute->GetExpAmount());
//
//			// Effect 발동 명령.
//			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
//		}
//	}
//
//	// 어빌리티 종료.
//	bool bReplicatedEndAbility = true;
//	bool bWasCancelled = false;
//	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
//}

void UVSGA_GiveExp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}
