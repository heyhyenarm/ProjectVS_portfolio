// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSBaseSkill.h"

// Sets default values
AVSBaseSkill::AVSBaseSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVSBaseSkill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVSBaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

