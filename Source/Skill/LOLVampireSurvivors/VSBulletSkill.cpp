// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSBulletSkill.h"
#include "NiagaraComponent.h"
#include "VSBulletSkill.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "ProjectLOL.h"
#include "Physics/LCCollisiion.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"

// Sets default values
AVSBulletSkill::AVSBulletSkill()
	: AttackRadius(200.f), AttackTime(5.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVSBulletSkill::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AVSBulletSkill::Damage, 10.f, true, 0.2f);
}

// Called every frame
void AVSBulletSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldTransform(FTransform(FVector(100.f, 0.f, 0.f)));
}

void AVSBulletSkill::Damage()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	//AttackingTime = 0.f;

	if (GetWorld()->GetTimerManager().IsTimerActive(OverlapHandle))
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		OverlapHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				AttackingTime += 0.5f;

				TArray<AActor*> OverlappedActors;

				// 충돌 감지 설정.
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel11));

				TArray<AActor*> IgnoreActors;

				IgnoreActors.Add(GetOwner());

				//LOL_LOG(LogProjectLOL, Log, TEXT("AttackingTime %f"), AttackingTime);

				UKismetSystemLibrary::SphereOverlapActors(
					GetWorld(),
					GetOwner()->GetActorLocation(),
					AttackRadius,
					ObjectTypes,
					AActor::StaticClass(),
					IgnoreActors,
					OverlappedActors
				);

#if ENABLE_DRAW_DEBUG

				FColor DrawColor = OverlappedActors.IsEmpty() ? FColor::Red : FColor::Green;
				DrawDebugSphere(
					GetWorld(),
					GetOwner()->GetActorLocation(),
					AttackRadius,
					12,
					DrawColor,
					false,
					0.5f);

#endif
				UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

				for (AActor* Target : OverlappedActors)
				{
					if (Target == GetOwner())
					{
						continue;
					}
					else if (IsValid(Cast<AVSBoidCharacter>(Target)))
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

				if (AttackingTime >= AttackTime)
				{
					//LOL_LOG(LogProjectLOL, Log, TEXT("AttackingTime Over"));

					GetWorld()->GetTimerManager().ClearTimer(OverlapHandle);
					AttackingTime = 0.f;
					return;
				}

			}), 0.05f, true
	);
}

