// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LOLVampireSurvivors/VSUserWidget.h"
#include "VSHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSHPBarWidget : public UVSUserWidget
{
	GENERATED_BODY()

public:
	UVSHPBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

// HP.
public:
	void UpdateMaxHP(const float BaseHP, const float ModifierHP);

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

	FString GetHpStatText();

// Exp.
public:
	void UpdateMaxExp(const float BaseExp, const float ModifierExp);

	UFUNCTION()
	void UpdateExpBar(float NewCurrentExp, float NewMaxExp);

	FString GetExpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	TObjectPtr<class UProgressBar> ExpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> ExpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float CurrentExp;

	UPROPERTY()
	float MaxExp;

	bool IsHPBar;

};
