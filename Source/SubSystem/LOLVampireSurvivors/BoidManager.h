// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"
#include "BoidManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UBoidManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
    explicit UBoidManager();

    virtual void PostInitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UBoidManagerSubsystem, STATGROUP_Tickables); }

public:
    // ��� Boid ����.
    UPROPERTY()
    TArray<AVSBoidCharacter*> Boids;

    // �÷��̾� Ÿ�� ����Ʈ.
    UPROPERTY()
    TArray<ACharacter*> PlayerTargets;

    // Boid ��� �Լ�.
    UFUNCTION(BlueprintCallable, Category = "Flock")
    void RegisterBoid(AVSBoidCharacter* Boid);

    // Boid ��� ���� �Լ�.
    UFUNCTION(BlueprintCallable, Category = "Flock")
    void UnregisterBoid(AVSBoidCharacter* Boid);

    // ���� �� �÷��̾� ĳ���� ����Ʈ ���� �Լ�.
    void UpdatePlayerTargets();

    // Boid �̿� update �Լ�.
    void UpdateBoidNeighbourhoods();

    // ������ ��ġ�� Boid ���� �Լ�.
    void SpawnBoidAtRandom(const FVector& PlayerLocation);

    // �÷��̾� �ݰ� ������ ��ġ �޾ƿ��� �Լ�.
    FVector GetRandomSpawnPositionAroundPlayer(const FVector& PlayerLocation, float MinRadius, float MaxRadius);

    FORCEINLINE AVSBoidCharacter* GetAutoTargetBoid() { return AutoTargetBoid; }

// Boid.
protected:
    UPROPERTY(EditAnywhere, Category = Boid)
    TSubclassOf<AVSBoidCharacter> BoidClass;

    UPROPERTY(EditAnywhere, Category = Boid)
    int32 SpawnBoidNum;

    UPROPERTY(EditAnywhere, Category = Boid)
    int32 MaxBoidNum;

    // �̿��� �����ϸ� �÷��̾ ���� ����� Boid ����.
    UPROPERTY(EditAnywhere, Category = Boid)
    TObjectPtr<AVSBoidCharacter> AutoTargetBoid;

    FVector PlayerLocation;

    FTimerHandle SpawnTimerHandle;

// Object Pooling.
protected:
    UPROPERTY()
    TObjectPtr<class ULCObjectPoolingSubsystem> ObjectPool;

};
