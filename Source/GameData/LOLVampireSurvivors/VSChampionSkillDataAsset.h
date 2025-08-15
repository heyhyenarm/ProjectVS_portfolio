// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystemComponent.h"
#include "VSChampionSkillDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FChampionSkillInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> SkillGA;

	// UI.
	UPROPERTY(EditAnywhere)
	FString SkillName;

	UPROPERTY(EditAnywhere)
	FString Description;
};

USTRUCT(BlueprintType)
struct FChampionUpgradeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> UpgradeGA;

	// UI.
	UPROPERTY(EditAnywhere)
	FString UpgradeName;

	UPROPERTY(EditAnywhere)
	FString Description;
};




/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSChampionSkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	explicit UVSChampionSkillDataAsset();

	//TArray<FChampionSkillInfo> GetRandomSkill();

protected:
	UPROPERTY(EditAnywhere)
	TArray<FChampionSkillInfo> SkillList;

	UPROPERTY(EditAnywhere)
	TArray<FChampionUpgradeInfo> UpgradeList;
};
