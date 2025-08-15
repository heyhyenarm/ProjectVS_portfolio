// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VSEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOnDead(uint8 bInIsDead) { bIsDead = bInIsDead; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	uint8 bIsDead : 1;
};
