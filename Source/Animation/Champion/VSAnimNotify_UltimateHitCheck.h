// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "VSAnimNotify_UltimateHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSAnimNotify_UltimateHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	explicit UVSAnimNotify_UltimateHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

};
