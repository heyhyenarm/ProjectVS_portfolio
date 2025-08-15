// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"

#include "VSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API AVSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	explicit AVSPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PostInitializeComponents() override;

public:
	FORCEINLINE class UVSChampionAttributeSet* GetChampionAttributeSet() { return AttributeSet; }

protected:

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UVSChampionAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<class UVSExpAttributeSet> ExpAttributeSet;

};
