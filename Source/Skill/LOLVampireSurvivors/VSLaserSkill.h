// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/LOLVampireSurvivors/VSBaseSkill.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "VSLaserSkill.generated.h"

UCLASS()
class PROJECTLOL_API AVSLaserSkill : public AVSBaseSkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit AVSLaserSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Damage();

protected:
	void ActivateNaiagara();
	void DeActivateNaiagara();

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackRadius;

	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackTime;

	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TArray<TObjectPtr<UNiagaraComponent>> NiagaraList;

	FTimerHandle DamageTimerHandle;
	FTimerHandle OverlapHandle;

	float AttackingTime = 0.f;
};
