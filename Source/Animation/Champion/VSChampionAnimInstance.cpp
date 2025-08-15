// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LOLVampireSurvivors/Champion/VSChampionAnimInstance.h"
#include "VSChampionAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UVSChampionAnimInstance::UVSChampionAnimInstance()
{
	MovingThreshold = 3.f;
	bOnAttack = 0;
}

void UVSChampionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UVSChampionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		GroundSpeed = Movement->Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
	}
}