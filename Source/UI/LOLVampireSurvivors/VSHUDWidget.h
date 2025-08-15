// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LOLVampireSurvivors/VSUserWidget.h"
#include "UI/LOLVampireSurvivors/VSLevelUpWidget.h"
#include "VSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSHUDWidget : public UVSUserWidget
{
	GENERATED_BODY()
	
public:
	explicit UVSHUDWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UVSLevelUpWidget* GetLevelUpWidget() { return LevelUpWidget; }

public:
	//void UpdateLevelUpWidget();
	//void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

protected:
	UFUNCTION()
	void OnGameOver();

public:
	//UPROPERTY()
	//TObjectPtr<class UABHpBarWidget> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVSLevelUpWidget> LevelUpWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVSHPBarWidget> HPBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVSHPBarWidget> ExpBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> GameOverPanel;
};
