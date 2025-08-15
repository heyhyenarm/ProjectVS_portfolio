                     // Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"
#include "SubSystem/LOLVampireSurvivors/BoidManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "ProjectLOL.h"

// Sets default values
AVSBoidCharacter::AVSBoidCharacter()
    : bUseFlocking(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    bIsTargetStop = false;
}

// Called when the game starts or when spawned
void AVSBoidCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        ObjectPool = GetWorld()->GetSubsystem<ULCObjectPoolingSubsystem>();
    }

    if (UBoidManager* Manager = GetWorld()->GetSubsystem<UBoidManager>())
    {
        Manager->RegisterBoid(this);
    }

    GetCharacterMovement()->MaxWalkSpeed = 400.0f;

    UCharacterMovementComponent* MoveComponent = GetCharacterMovement();
    //LOL_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed: %f, MovementMode: %d, IsActive: %d"), MoveComponent->MaxWalkSpeed, (int32)MoveComponent->MovementMode, MoveComponent->IsActive());
}

void AVSBoidCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UBoidManager* Manager = GetWorld()->GetSubsystem<UBoidManager>())
    {
        Manager->UnregisterBoid(this);
    }
    Super::EndPlay(EndPlayReason);
}

// Called every frame
void AVSBoidCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bUseFlocking)
    {
        CalculateFlocking();

        //LOL_LOG(LogTemp, Warning, TEXT("NewMoveVector %f %f %f"), NewMoveVector.X, NewMoveVector.Y, NewMoveVector.Z);

        // 이동.
        if (!NewMoveVector.IsNearlyZero())
        {
            FVector MoveDirection = NewMoveVector.GetSafeNormal();
            AddMovementInput(MoveDirection, 1.0f);

            if (!bIsTargetStop)
            {
                // 회전.
                FRotator TargetRotation = UKismetMathLibrary::MakeRotFromXZ(MoveDirection, FVector::UpVector);
                FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, MaxRotationSpeed);
                SetActorRotation(SmoothRotation);

                //LOL_LOG(LogProjectLOL, Warning, TEXT("Now Location %f %f %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
            }
            else
            {
                // 플레이어가 멈춰있는 상태. 플레이어 바라보기. 
                AAIController* AIController = Cast<AAIController>(GetController());
                if (IsValid(AIController))
                {
                    AIController->SetFocus(PlayerTargets[0]);
                }
            }
        }
    }
    // else: BT 실행.
}

// Called to bind functionality to input
void AVSBoidCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVSBoidCharacter::ReturnToPool()
{
    GetWorld()->GetSubsystem<ULCObjectPoolingSubsystem>()->ReturnToPool(this);
}

void AVSBoidCharacter::CalculateFlocking()
{
    //LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

    ResetComponents();

    // 정렬 벡터 계산.
    CalculateAlignmentComponentVector();

    if (Neighbourhood.Num() > 0)
    {
        //LOL_LOG(LogProjectLOL, Log, TEXT("Neighbourhood %d"), Neighbourhood.Num());

        // 응집력, 분리 벡터 계산.
        CalculateCohesionComponentVector();
        CalculateSeparationComponentVector();
    }

    // Collision 벡터 계산.
    CalculateCollisionComponentVector();

    // 가장 가까운 플레이어를 타겟으로 함.
    FVector TargetForce = FVector::ZeroVector;
    if (PlayerTargets.Num() > 0)
    {

        if (TargetPreLocation.X == 0.f && TargetPreLocation.Y == 0.f)
        {
            TargetPreLocation = PlayerTargets[0]->GetActorLocation();
        }

        //LOL_LOG(LogProjectLOL, Log, TEXT("PlayerTargets %d"), PlayerTargets.Num());
        //LOL_LOG(LogProjectLOL, Log, TEXT("PlayerTarget TargetPreLocation %f %f %f"), TargetPreLocation.X, TargetPreLocation.Y, TargetPreLocation.Z);

        float MinDistance = TNumericLimits<float>::Max();
        FVector TargetLocation = PlayerTargets[0]->GetActorLocation();
        //LOL_LOG(LogProjectLOL, Log, TEXT("PlayerTarget Location %f %f %f"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z);

        if (!bIsTargetStop)
        {
            if (TargetLocation == TargetPreLocation)
            {
                bIsTargetStop = true;
            }
        }
        else
        {
            if (TargetLocation != TargetPreLocation)
            {
                bIsTargetStop = false;
            }
        }
        TargetPreLocation = TargetLocation;

        for (ACharacter* Target : PlayerTargets)
        {
            float Distance = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                TargetLocation = Target->GetActorLocation();
            }
        }
        TargetForce = (TargetLocation - GetActorLocation()).GetSafeNormal() * TargetWeight;
    }
    NewMoveVector = AlignmentComponent + CohesionComponent + SeparationComponent + CollisionComponent + TargetForce;
    NewMoveVector.Z = 0.0f;
    CurrentMoveVector = NewMoveVector.GetSafeNormal();

    //Compute();

}

void AVSBoidCharacter::ResetComponents()
{
    AlignmentComponent = FVector::ZeroVector;
    CohesionComponent = FVector::ZeroVector;
    SeparationComponent = FVector::ZeroVector;
    CollisionComponent = FVector::ZeroVector;
}

void AVSBoidCharacter::CalculateAlignmentComponentVector()
{
    for (const AVSBoidCharacter* Boid : Neighbourhood)
    {
        AlignmentComponent += Boid->CurrentMoveVector.GetSafeNormal(DefaultNormalizeVectorTolerance);
    }
    AlignmentComponent = (CurrentMoveVector + AlignmentComponent).GetSafeNormal(DefaultNormalizeVectorTolerance) * AlignmentWeight;
}

void AVSBoidCharacter::CalculateCohesionComponentVector()
{
    const FVector& Location = GetActorLocation();
    for (const AVSBoidCharacter* Boid : Neighbourhood)
    {
        CohesionComponent += Boid->GetActorLocation() - Location;
    }
    CohesionComponent = (CohesionComponent / Neighbourhood.Num() / CohesionLerp) * CohesionWeight;
}

void AVSBoidCharacter::CalculateSeparationComponentVector()
{
    const FVector& Location = GetActorLocation();
    for (const AVSBoidCharacter* Boid : Neighbourhood)
    {
        FVector Separation = Location - Boid->GetActorLocation();
        SeparationComponent += Separation.GetSafeNormal(DefaultNormalizeVectorTolerance) / FMath::Abs(Separation.Size() - BoidPhysicalRadius);
    }

    FVector SeparationForceComponent = SeparationComponent * SeparationForce;
    SeparationComponent += (SeparationForceComponent + SeparationForceComponent * (SeparationLerp / Neighbourhood.Num())) * SeparationWeight;
}

void AVSBoidCharacter::CalculateCollisionComponentVector()
{
    FHitResult OutHit;
    const FVector& Location = GetActorLocation();
    FVector End = Location + GetActorForwardVector() * CollisionDistanceLook;
    FCollisionQueryParams Params(NAME_None, false, this);
    FCollisionShape SphereShape = FCollisionShape::MakeSphere(BoidPhysicalRadius);

    if (GetWorld()->SweepSingleByChannel(OutHit, Location, End, FQuat::Identity, ECC_WorldStatic, SphereShape, Params))
    {
        FVector Direction = OutHit.ImpactPoint - Location;
        CollisionComponent -= (Direction.GetSafeNormal(DefaultNormalizeVectorTolerance) / FMath::Abs(Direction.Size() - BoidPhysicalRadius)) * CollisionWeight;
    }
}

void AVSBoidCharacter::Compute()
{
    //NewMoveVector = AlignmentComponent + CohesionComponent + SeparationComponent + CollisionComponent;
    //// 수직이동 방지.
    //NewMoveVector.Z = 0.0f;
    //CurrentMoveVector = NewMoveVector.GetSafeNormal();

    //LOL_LOG(LogProjectLOL, Log, TEXT("CurrentMoveVector %f %f %f"), CurrentMoveVector.X, CurrentMoveVector.Y, CurrentMoveVector.Z);
}
