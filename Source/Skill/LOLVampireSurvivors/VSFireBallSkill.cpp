// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSFireBallSkill.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"
#include "Physics/LCCollisiion.h"
#include "ProjectLOL.h"

// Sets default values
AVSFireBallSkill::AVSFireBallSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set Collision.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComponent;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AVSFireBallSkill::OnComponentBeginOverlap);

	// set NiagaraComponent.
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireBall"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AVSFireBallSkill::BeginPlay()
{
	Super::BeginPlay();

	if (FireballEffect)
	{
		NiagaraComponent->SetAsset(FireballEffect);
		NiagaraComponent->Activate(true);
		NiagaraComponent->SetRelativeRotation(FRotator(-45.f, 45.f, 0.f));
		NiagaraComponent->SetRelativeScale3D(FVector::OneVector * 3.f);
	}

	// 지면을 향해 낙하. 
	FVector FallVelocity = FVector(0, 0, -FallSpeed);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	AddActorWorldOffset(FallVelocity * GetWorld()->GetDeltaSeconds(), true);

}

// Called every frame
void AVSFireBallSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector(1000 * DeltaTime, 1000 * DeltaTime, -FallSpeed * DeltaTime), true);
	//SetActorRotation(FRotator::ZeroRotator);
}

void AVSFireBallSkill::OnComponentBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Overlap Actor : %s"), *OtherActor->GetName());

	// 이펙트.
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	}

	// 데미지 처리.
	TArray<AActor*> HitActors;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), 
		GetActorLocation(), 
		AttackRadius,
		{ UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel11) },
		AActor::StaticClass(), 
		{ this }, 
		HitActors);

//#if ENABLE_DRAW_DEBUG
//
//	FColor DrawColor = HitActors.IsEmpty() ? FColor::Red : FColor::Green;
//	DrawDebugSphere(
//		GetWorld(),
//		GetActorLocation(),
//		AttackRadius,
//		12,
//		DrawColor,
//		false,
//		0.5f);
//
//#endif

	UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

	for (AActor* Target : HitActors)
	{
		if (IsValid(Cast<AVSBoidCharacter>(Target)))
		{
			//LOL_LOG(LogProjectLOL, Warning, TEXT("Target %s"), *Target->GetName());

			// 데미지 GE.
			UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
			if (TargetASC)
			{
				FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
				ContextHandle.AddInstigator(OwningActor, nullptr);

				FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, 1, ContextHandle);

				if (SpecHandle.IsValid())
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
				}
			}
		}
	}
	//Destroy();
}

