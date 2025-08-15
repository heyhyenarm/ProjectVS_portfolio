// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/LOLVampireSurvivors/BoidManager.h"
#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"
#include "Character/LOLVampireSurvivors/Enemy/TestBoidCharacter.h"

#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "ProjectLOL.h"
#include "Character/LOLVampireSurvivors/Enemy/VSEnemyTrashMob.h"

UBoidManager::UBoidManager()
    : SpawnBoidNum(10), MaxBoidNum(200)
{
    static ConstructorHelpers::FClassFinder<AVSBoidCharacter> BoidClassRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Enemy/TrashMob/BP_EnemyTrashMop.BP_EnemyTrashMop_C"));
    if (BoidClassRef.Succeeded())
    {
        BoidClass = BoidClassRef.Class;
    }
}

void UBoidManager::PostInitialize()
{
    Super::PostInitialize();

    //LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

    ObjectPool = GetWorld()->GetSubsystem<ULCObjectPoolingSubsystem>();

    if (ROLE_Authority == ENetRole::ROLE_Authority)
    {
        // NHRTodo: 스폰 테스트.
        GetWorld()->GetTimerManager().SetTimer(
            SpawnTimerHandle,  
            FTimerDelegate::CreateLambda([&]() {
                if (!PlayerTargets.IsEmpty())
                {
                    FVector PlayerLocation = PlayerTargets[0]->GetActorLocation();
                    for (int i = 0; i < SpawnBoidNum; ++i)
                    {
                        SpawnBoidAtRandom(FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z + 1000.f));

                        // 최대 Boid 한정.
                        if (Boids.Num() >= MaxBoidNum)
                        {
                            LOL_LOG(LogProjectLOL, Warning, TEXT("Max Boid"));

                            SpawnTimerHandle.Invalidate();
                        }
                    }
                }
            }), 10.0f, true, 0.f);
    }
}

void UBoidManager::RegisterBoid(AVSBoidCharacter* Boid)
{
	if (Boid && !Boids.Contains(Boid))
	{
		Boids.Add(Boid);
	}
}

void UBoidManager::UnregisterBoid(AVSBoidCharacter* Boid)
{
	Boids.Remove(Boid);
}

void UBoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (ROLE_Authority == ENetRole::ROLE_Authority)
    {
        UpdatePlayerTargets();
        UpdateBoidNeighbourhoods();
    }

    // NHRTodo: 스폰 테스트.
    //if (Boids.Num() <= 30)
    //{
    //    SpawnBoidAtRandom(PlayerTargets[0]->GetActorLocation());
    //}
}

void UBoidManager::UpdatePlayerTargets()
{
    PlayerTargets.Empty();
    for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        if (APlayerController* PC = Iterator->Get())
        {
            if (ACharacter* Character = Cast<ACharacter>(PC->GetPawn()))
            {
                PlayerTargets.Add(Character);
            }
        }
    }
}

void UBoidManager::UpdateBoidNeighbourhoods()
{
    float MinDistance = 10000000.f;

    for (AVSBoidCharacter* Boid : Boids)
    {
        // 플레이어와 가장 가까운 Boid 구하기.
        float BoidToPlayerDistance = FVector::DistSquared(PlayerLocation, Boid->GetActorLocation());

        if (BoidToPlayerDistance < MinDistance)
        {
            MinDistance = BoidToPlayerDistance;
            AutoTargetBoid = Boid;
        }

        Boid->Neighbourhood.Empty();
        FVector BoidLocation = Boid->GetActorLocation();
        for (AVSBoidCharacter* Other : Boids)
        {

            if (Other == Boid) continue;
            if (FVector::DistSquared(BoidLocation, Other->GetActorLocation()) < FMath::Square(Boid->VisionRadius))
            {
                Boid->Neighbourhood.Add(Other);
            }
        }
        // 현재 플레이어 타겟 리스트를 Boid에게 전달
        Boid->PlayerTargets = PlayerTargets;
    }
}

void UBoidManager::SpawnBoidAtRandom(const FVector& InPlayerLocation)
{
    //LOL_LOG(LogProjectLOL, Warning, TEXT("Begin"));

    PlayerLocation = InPlayerLocation;

    // 타겟 플레이어 주변 랜덤 위치 설정하기.
    FVector SpawnLocation = GetRandomSpawnPositionAroundPlayer(InPlayerLocation, 3000, 4000);

    // 오브젝트 풀링 스폰.
    AVSEnemyTrashMob* Boid = Cast<AVSEnemyTrashMob>(ObjectPool->DeferredSpawnFromPool(BoidClass, SpawnLocation, FRotator::ZeroRotator));
    //GetWorld()->SpawnActor<AVSBoidCharacter>(BoidClass, SpawnPos, FRotator::ZeroRotator);
    if (Boid)
    {
        Boid->Initialize();
        ObjectPool->FinishSpawn(Boid);
    }
}

FVector UBoidManager::GetRandomSpawnPositionAroundPlayer(const FVector& InPlayerLocation, float MinRadius, float MaxRadius)
{
    float Angle = FMath::FRandRange(0.f, 2 * PI);
    float Distance = FMath::FRandRange(MinRadius, MaxRadius);
    FVector Offset = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * Distance;
    return InPlayerLocation + Offset;
}