// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSLaserSkill.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"
#include "Physics/LCCollisiion.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Abilities/GameplayAbility.h"

#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"
#include "ProjectLOL.h"

// Sets default values
AVSLaserSkill::AVSLaserSkill()
	: AttackRadius(500.f), AttackTime(15.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVSLaserSkill::BeginPlay()
{
	Super::BeginPlay();

	//AttackTime = NiagaraList[0]->GetMaxSimTime();
	//LOL_LOG(LogProjectLOL, Log, TEXT("AttackTime %f"), AttackTime);

	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AVSLaserSkill::Damage, 7.f, true, 0.f);
}

// Called every frame
void AVSLaserSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector PlayerLocation = OwningActor->GetActorLocation();
	SetActorLocation(PlayerLocation);
	AddActorLocalRotation(FRotator(0.f, 0.f, 1.f * DeltaTime));
	//AttackingTime += DeltaTime;
	//Damage();
}

void AVSLaserSkill::Damage()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	//AVSCharacterChampionPlayer* Champion = Cast<AVSCharacterChampionPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//if (IsValid(Champion))
	//{
	//	Champion->GetAbilitySystemComponent()->
	//}

	ActivateNaiagara();

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

				IgnoreActors.Add(this);

				//LOL_LOG(LogProjectLOL, Log, TEXT("AttackingTime %f"), AttackingTime);

				UKismetSystemLibrary::SphereOverlapActors(
					GetWorld(),
					GetActorLocation(),
					AttackRadius,
					ObjectTypes,
					AActor::StaticClass(),
					IgnoreActors,
					OverlappedActors
				);

//#if ENABLE_DRAW_DEBUG
//
//				FColor DrawColor = OverlappedActors.IsEmpty() ? FColor::Red : FColor::Green;
//				DrawDebugSphere(
//					GetWorld(),
//					GetActorLocation(),
//					AttackRadius,
//					12,
//					DrawColor,
//					false,
//					0.5f);
//
//#endif

				UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

				for (AActor* Target : OverlappedActors)
				{
					if (Target == GetOwner())
					{
						continue;
					}
					else if (IsValid(Cast<AVSBoidCharacter>(Target)))
					{
						LOL_LOG(LogProjectLOL, Error, TEXT("Target %s"), *Target->GetName());

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
					OverlapHandle.Invalidate();
				}

			}), 0.05f, true, 0.f
	);
}

void AVSLaserSkill::ActivateNaiagara()
{
	for (auto Niagara : NiagaraList)
	{
		if (Niagara->IsActive())
		{
			continue;
		}
		Niagara->Activate();
	}
}

void AVSLaserSkill::DeActivateNaiagara()
{
	for (auto Niagara : NiagaraList)
	{
		if (Niagara->IsActive())
		{
			Niagara->Deactivate();
		}
	}
}


