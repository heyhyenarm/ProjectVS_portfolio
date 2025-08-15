// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VSGameSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSGameSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	FOnGameOver OnGameOverDelegate;

};
