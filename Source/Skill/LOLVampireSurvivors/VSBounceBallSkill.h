// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "VSBounceBallSkill.generated.h"

UCLASS()
class PROJECTLOL_API AVSBounceBallSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVSBounceBallSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//void Damage();

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackingTime;

	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<class UGameplayEffect> DamageEffect;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	FTimerHandle DamageTimerHandle;

};
