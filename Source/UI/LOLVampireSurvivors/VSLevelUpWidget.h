// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LOLVampireSurvivors/VSUserWidget.h"
#include "VSLevelUpWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetSkillDelegate);
/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSLevelUpWidget : public UVSUserWidget
{
	GENERATED_BODY()

public: 
	explicit UVSLevelUpWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	void Hide();
	
	UFUNCTION(BlueprintCallable)
	void UpdateSkiils();

	FGetSkillDelegate OnGetSkillDone;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> LevelUpPanel;

	UPROPERTY(meta = (BindWidget))
	TArray<TObjectPtr<class UButton>> SkillButtonList;

	//UPROPERTY()
	//class UVSCharacterAttributeSet* CharacterAttributeSet;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AVSPlayerState> VSPlayerState;
};
