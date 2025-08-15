// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/LOLClassic/LCPoolInterface.h"
#include "VSBoidCharacter.generated.h"

UCLASS()
class PROJECTLOL_API AVSBoidCharacter : public ACharacter, public ILCPoolInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit AVSBoidCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
    void ReturnToPool();

public:

    // BoidManager를 통하여 받아올 정보들.
    // 주변 Boid 리스트.
    TArray<AVSBoidCharacter*> Neighbourhood;
    // 타겟 Player 리스트.
    TArray<ACharacter*> PlayerTargets;

    // 각 행동 규칙 벡터.
    // Boid의 상태를 나타냄.
    // 새 이동 벡터는 다음 Component들의 합을 이용하여 구함.
    // 정렬 벡터.
    FVector AlignmentComponent;
    // 응집력 벡터.
    FVector CohesionComponent;
    // 분리 벡터.
    FVector SeparationComponent;

    FVector CollisionComponent;
    FVector CurrentMoveVector;
    FVector NewMoveVector;

    // Flocking 파라미터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float AlignmentWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float CohesionWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float CohesionLerp = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float SeparationWeight = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float SeparationLerp = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float SeparationForce = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float CollisionWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float TargetWeight = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float VisionRadius = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float CollisionDistanceLook = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float MaxRotationSpeed = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float MaxMovementSpeed = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float BaseMovementSpeed = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock")
    float BoidPhysicalRadius = 45.0f;

    // Debug.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flock|Debug")
    bool bEnableDebugDraw = false;

protected:
    // Tick 마다 호출.
    void CalculateFlocking();

    void ResetComponents();

    // 정렬 벡터 계산.
    void CalculateAlignmentComponentVector();
    // 응집력 벡터 계산. 
    void CalculateCohesionComponentVector();
    // 분리 벡터 계산.
    void CalculateSeparationComponentVector();
    // 장애물 회피 벡터 계산.
    void CalculateCollisionComponentVector();

    // 합산.
    void Compute();

    // 오차용.
    float DefaultNormalizeVectorTolerance = 0.0001f;

protected:
    UPROPERTY()
    TObjectPtr<class ULCObjectPoolingSubsystem> ObjectPool;

    FVector TargetPreLocation;

public:
// Dirty.
    uint8 bUseFlocking : 1;
    uint8 bIsTargetStop : 1;
    uint8 bIsLookingTarget : 1;
};