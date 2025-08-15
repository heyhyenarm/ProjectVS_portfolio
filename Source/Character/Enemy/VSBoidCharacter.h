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

    // BoidManager�� ���Ͽ� �޾ƿ� ������.
    // �ֺ� Boid ����Ʈ.
    TArray<AVSBoidCharacter*> Neighbourhood;
    // Ÿ�� Player ����Ʈ.
    TArray<ACharacter*> PlayerTargets;

    // �� �ൿ ��Ģ ����.
    // Boid�� ���¸� ��Ÿ��.
    // �� �̵� ���ʹ� ���� Component���� ���� �̿��Ͽ� ����.
    // ���� ����.
    FVector AlignmentComponent;
    // ������ ����.
    FVector CohesionComponent;
    // �и� ����.
    FVector SeparationComponent;

    FVector CollisionComponent;
    FVector CurrentMoveVector;
    FVector NewMoveVector;

    // Flocking �Ķ����
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
    // Tick ���� ȣ��.
    void CalculateFlocking();

    void ResetComponents();

    // ���� ���� ���.
    void CalculateAlignmentComponentVector();
    // ������ ���� ���. 
    void CalculateCohesionComponentVector();
    // �и� ���� ���.
    void CalculateSeparationComponentVector();
    // ��ֹ� ȸ�� ���� ���.
    void CalculateCollisionComponentVector();

    // �ջ�.
    void Compute();

    // ������.
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