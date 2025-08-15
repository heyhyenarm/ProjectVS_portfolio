// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerSkillSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UPlayerSkillSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> TestAbilities;
};
