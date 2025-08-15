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
    // 모든 Boid 관리.
    UPROPERTY()
    TArray<AVSBoidCharacter*> Boids;

    // 플레이어 타겟 리스트.
    UPROPERTY()
    TArray<ACharacter*> PlayerTargets;

    // Boid 등록 함수.
    UFUNCTION(BlueprintCallable, Category = "Flock")
    void RegisterBoid(AVSBoidCharacter* Boid);

    // Boid 등록 해제 함수.
    UFUNCTION(BlueprintCallable, Category = "Flock")
    void UnregisterBoid(AVSBoidCharacter* Boid);

    // 월드 내 플레이어 캐릭터 리스트 갱신 함수.
    void UpdatePlayerTargets();

    // Boid 이웃 update 함수.
    void UpdateBoidNeighbourhoods();

    // 랜덤한 위치에 Boid 스폰 함수.
    void SpawnBoidAtRandom(const FVector& PlayerLocation);

    // 플레이어 반경 랜덤한 위치 받아오는 함수.
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

    // 이웃을 팀색하며 플레이어에 가장 가까운 Boid 저장.
    UPROPERTY(EditAnywhere, Category = Boid)
    TObjectPtr<AVSBoidCharacter> AutoTargetBoid;

    FVector PlayerLocation;

    FTimerHandle SpawnTimerHandle;

// Object Pooling.
protected:
    UPROPERTY()
    TObjectPtr<class ULCObjectPoolingSubsystem> ObjectPool;

};
