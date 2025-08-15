// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/LOLVampireSurvivors/VSBaseSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "VSFireBallSkill.generated.h"

UCLASS()
class PROJECTLOL_API AVSFireBallSkill : public AVSBaseSkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit AVSFireBallSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	UNiagaraSystem* FireballEffect;

	UPROPERTY(EditAnywhere, Category = Attack)
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = Attack)
	TSubclassOf<class UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, Category = Attack)
	float FallSpeed = 1000.f;

	//UPROPERTY(VisibleAnywhere, Category = Attackattac)
	//UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponent;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
