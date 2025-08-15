// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/LOLVampireSurvivors/VSBaseSkill.h"
#include "VSBulletSkill.generated.h"

UCLASS()
class PROJECTLOL_API AVSBulletSkill : public AVSBaseSkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit AVSBulletSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Damage();

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackRadius;

	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackTime;

	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<class UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UNiagaraComponent> Niagara;

	FTimerHandle DamageTimerHandle;
	FTimerHandle OverlapHandle;

	float AttackingTime = 0.f;

};
