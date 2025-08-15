// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VSChampionAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSChampionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	explicit UVSChampionAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	FORCEINLINE uint8 GetOnAttack() { return bOnAttack; };
	FORCEINLINE void SetOnAttack(uint8 bInOnAttack) { bOnAttack = bInOnAttack; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	uint8 bOnAttack : 1;
};
