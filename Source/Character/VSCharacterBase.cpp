// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LOLVampireSurvivors/VSCharacterBase.h"

#include "Components/CapsuleComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

#include "Physics/LCCollisiion.h"

// Sets default values
AVSCharacterBase::AVSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CHARACTER);

}

// Called when the game starts or when spawned
void AVSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

void AVSCharacterBase::OnOutOfHP()
{
}

