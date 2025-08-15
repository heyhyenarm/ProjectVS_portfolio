// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LOLVampireSurvivors/Enemy/VSEnemyTrashMob.h"
#include "VSEnemyTrashMob.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"
//#include "Prob/LOLVampireSurvivors/VSItemExp.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

//#include "AbilitySystemComponent.h"
#include "Attribute/VSCharacterAttributeSet.h"

#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "ProjectLOL.h"

// Sets default values
AVSEnemyTrashMob::AVSEnemyTrashMob()
{
	// ASC 생성.
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	// AttributeSet 생성.
	AttributeSet = CreateDefaultSubobject<UVSCharacterAttributeSet>(TEXT("AttributeSet"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// set skeletalmesh.
	//TrashMobBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Game/MonsterForSurvivalGame/Mesh/PBR/Swarm09_SK.Swarm09_SK"));
	if (SkeletalMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
		GetMesh()->GetRelativeTransform().SetScale3D(FVector::OneVector * 0.3f);
		GetMesh()->GetRelativeTransform().SetLocation(FVector(0.f, 0.f, -15.f));
		//RootComponent = GetMesh();

		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(3);
	}

	// set AnimInstance.
	//static ConstructorHelpers::FObjectFinder<UAnimInstance> TrashMobAnimInstanceRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Enemy/TrashMob/Animations/VSBP_EnemyTrashMob.VSBP_EnemyTrashMob"));
	//if (TrashMobAnimInstanceRef.Succeeded())
	//{
	//	TrashMobAnimInstance = TrashMobAnimInstanceRef.Object;
	//}

	// create Attack Collision.
	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetSphereRadius(30.f);
	AttackCollision->SetupAttachment(RootComponent);

	bOnOverlap = false;
	bOnDead = false;
}

// Called when the game starts or when spawned
void AVSEnemyTrashMob::BeginPlay()
{
	Super::BeginPlay();
	
	//check(TrashMobAnimInstance = Cast<UAnimInstance>(TrashMobBody->GetAnimInstance()));
}

void AVSEnemyTrashMob::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Actor spawn 이후, Component들이 준비된 후에 ASC 초기화.
	ASC->InitAbilityActorInfo(this, this);

	AttributeSet->OnOutOfHP.AddDynamic(this, &AVSEnemyTrashMob::OnOutOfHP);

	// 발동할 GA 부여.
	for (const auto& StartAbility : StartAbilites)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

UAbilitySystemComponent* AVSEnemyTrashMob::GetAbilitySystemComponent() const
{
	return ASC;
}

void AVSEnemyTrashMob::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//Attack();

	AVSCharacterChampionPlayer* Champion = Cast< AVSCharacterChampionPlayer>(OtherActor);
	if (IsValid(Champion))
	{
		bUseFlocking = false;
		bOnOverlap = true;

		//LOL_LOG(LogProjectLOL, Warning, TEXT("Actor Overlap %s Begin"), *OtherActor->GetName());
		//LOL_LOG(LogProjectLOL, Log, TEXT("NotifyActorBeginOverlap Begin"));

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					if (bOnOverlap)
					{
						Attack();
					}
					else
					{
						if (!bOnDead)
						{
							ASC->CancelAbilities();
							bUseFlocking = true;
						}
						TimerHandle.Invalidate();
					}
				}),
			AttackCoolTime,
			bOnOverlap
		);
	}
}

void AVSEnemyTrashMob::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	//LOL_LOG(LogProjectLOL, Warning, TEXT("Actor Overlap %s End"), *OtherActor->GetName());

	//FGameplayAbilitySpec TriggerAttackSpec(TriggerAttackAbility);
	//FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(TriggerAttackSpec);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Attack GA End"));
	//ASC->CancelAbilities();

	bOnOverlap = false;
}

// Called every frame
void AVSEnemyTrashMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVSEnemyTrashMob::Initialize()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	bOnOverlap = false;
	bOnDead = false;
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);

	AttributeSet->SetHP(AttributeSet->GetMaxHP());
	AttributeSet->bOutOfHP = false;

	ASC->CancelAbilities();

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AVSEnemyTrashMob::Attack()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Attack GA Begin"));

	FGameplayAbilitySpec TriggerAttackSpec(TriggerAttackAbility);
	AttackHandle = ASC->GiveAbility(TriggerAttackSpec);

	ASC->TryActivateAbility(AttackHandle);
}

void AVSEnemyTrashMob::Dead()
{
	LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
	SetActorEnableCollision(false);


	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	GetMesh()->GetAnimInstance()->Montage_Play(DeadMontage, 1.0f);
	float MontageTime = DeadMontage->GetPlayLength();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
				SetActorHiddenInGame(true);

				bOnOverlap = false;
				bOnDead = true;

				ASC->CancelAbilities();
				ASC->ClearAllAbilities();

				TimerHandle.Invalidate();
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

				// 오브젝트 풀로 반환.
				//Destroy();
				ObjectPool->ReturnToPool(this);
			}), MontageTime, false
	);
}

void AVSEnemyTrashMob::OnOutOfHP()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
	if (!bOnDead)
	{
		Dead();
	}
}
