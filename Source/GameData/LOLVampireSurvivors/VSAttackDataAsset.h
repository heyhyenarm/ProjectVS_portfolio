// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VSAttackDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSAttackDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UVSAttackDataAsset();

	// Animation.
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* DefaultAttackMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	FString DefaultAttackMontageSectionName;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* SkillAttackMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	FString SkillAttackMontageSectionName;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* UltimateSkillAttackMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	FString UltimateSkillAttackMontageSectionName;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	FString DeadMontageSectionName;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	float DefaultAttackTime;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	float SkillAttackTime;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	float UltimateSkillAttackTime;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	float DeadTime;

	// Stat.
	UPROPERTY(EditAnywhere, Category = Stat)
	float DefaultAttackCoolTime;

	UPROPERTY(EditAnywhere, Category = Stat)
	float SkillAttackCoolTime;

	UPROPERTY(EditAnywhere, Category = Stat)
	float UltimateSkillAttackCoolTime;

};
