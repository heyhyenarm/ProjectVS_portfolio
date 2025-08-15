#pragma once

#include "GameplayTagContainer.h"

#define VSTAG_CHAMPION_DEFAULTATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.Action.DefaultAttack"))
#define VSTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define VSTAG_CHARACTER_DEAD FGameplayTag::RequestGameplayTag(FName("VSCharacter.State.IsDead"))
#define VSTAG_CHAMPION_GAINEXP FGameplayTag::RequestGameplayTag(FName("VSChampion.Action.GainExp"))
#define VSTAG_CHAMPION_FIREBALLATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.State.IsFireballAttacking"))
#define VSTAG_CHAMPION_SHIELDATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.State.IsShieldAttacking"))
#define VSTAG_CHAMPION_LASERATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.State.IsLaserAttacking"))
#define VSTAG_CHAMPION_DEFAULTATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.State.IsDefaultAttacking"))
#define VSTAG_CHAMPION_SKILLTATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.State.IsSkillAttacking"))
#define VSTAG_CHAMPION_ULTIMATESKILLTATTACK FGameplayTag::RequestGameplayTag(FName("VSChampion.State.IsUltimateSkillAttacking"))