// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VSPlayerState.h"
#include "Attribute/VSChampionAttributeSet.h"
#include "Attribute/VSExpAttributeSet.h"

#include "AbilitySystemComponent.h"

AVSPlayerState::AVSPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	// PlayerState를 계속 Replicate 되도록 설정. 
	ASC->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UVSChampionAttributeSet>(TEXT("AttributeSet"));
	//SkillAttributeSet = CreateDefaultSubobject<UVSChampionSkillAttributeSet>(TEXT("SkillAttributeSet"));
	//UltimateSkillAttributeSet = CreateDefaultSubobject<UVSChampionUltimateAttributeSet>(TEXT("UltimateSkillAttributeSet"));
	ExpAttributeSet = CreateDefaultSubobject<UVSExpAttributeSet>(TEXT("ExpAttributeSet"));
}

UAbilitySystemComponent* AVSPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void AVSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//for (const auto& TestAbility : TestAbilities)
	//{
	//	FGameplayAbilitySpec TestSpec(TestAbility);
	//	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(TestSpec);
	//	ASC->TryActivateAbility(Handle);
	//}
}
