// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/LOLVampireSurvivors/VSChampionSkillDataAsset.h"
#include "VSLevelUpManager.generated.h"


USTRUCT()
struct FLevelupOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	uint8 bIsSkill : 1;

	UPROPERTY(EditAnywhere)
	FChampionSkillInfo SkillInfo;

	UPROPERTY(EditAnywhere)
	FChampionUpgradeInfo UpgradeInfo;
};


/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSLevelUpManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FORCEINLINE class UVSHUDWidget* GetHUDWidget() { return HUDWidget; }

public:
	UFUNCTION(BlueprintCallable)
	void OnLevelUp(class AVSCharacterChampionPlayer* Champion);

protected:
	UPROPERTY(EditAnywhere)
	TArray<FLevelupOption> LevelupOptions;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataAsset> ChampionSkillDataAsset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UVSHUDWidget> HUDWidget;

public:
	void SetupHUDWidget(class UVSHUDWidget* InHUDWidget);

};
