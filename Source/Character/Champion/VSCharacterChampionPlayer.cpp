// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"

#include "TimerManager.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"

#include "Animation/AnimationAsset.h"
#include "Animation/LOLVampireSurvivors/Champion/VSChampionAnimInstance.h"

#include "Components/BoxComponent.h"
#include "AbilitySystemComponent.h"

#include "Player/VSPlayerState.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

#include "GA/Attack/VSGA_Attack.h"
#include "GA/Attack/VSGA_UnionSkill.h"
#include "GameData/LOLVampireSurvivors/VSAttackDataAsset.h"
#include "Attribute/VSChampionAttributeSet.h"

#include "UI/LOLVampireSurvivors/VSHUDWidget.h"

#include "ProjectLOL.h"
#include "Tag/VSGameplayTag.h"

#include "SubSystem/LOLClassic/LCObjectPoolingSubsystem.h"
#include "SubSystem/LOLVampireSurvivors/VSGameSubsystem.h"
#include "SubSystem/LOLVampireSurvivors/VSLevelUpManager.h"


AVSCharacterChampionPlayer::AVSCharacterChampionPlayer()
{
	bReplicates = true;

	// ���� �÷��̾ ������ ������ PlayerState�� ASC ���� ������ ��. 
	ASC = nullptr;

	// set controller.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// set mesh component. 
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// @NHRTodo: è�Ǿ� �� �������Ʈ�� ����� -> ������ �������� �����ϱ�.
	// set skeletal mesh.
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/LeagueOfLegends/Character/Champions/Rammus/rammus.rammus"));
	//if (CharacterMeshRef.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	//}
	// set reference in blueprint. 
	//ensure(GetMesh());
	//GetMesh()->SetSkeletalMesh(ChampionSkeletalMesh.Get());

	// set animation asset.
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstancetClassRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Champions/Rammus/Animations/VSBP_Rammus.VSBP_Rammus_C"));
	//if (AnimInstancetClassRef.Succeeded())
	//{
	//	ensure(AnimInstancetClassRef.Class);
	//	GetMesh()->SetAnimClass(AnimInstancetClassRef.Class);
	//	//RammusAnimInstance = Cast<UVSRammusAnimInstance>(GetMesh()->GetAnimInstance());
	//}
	// set reference in blueprint. 
	//ensure(GetMesh()->GetAnimInstance);
	//if (ChampionAnimInstance = Cast< UVSChampionAnimInstance>(ChampionAnimationAsset.GetClass()))
	//{
	//	GetMesh()->SetAnimClass(ChampionAnimInstance.GetClass());
	//}

	// add attack collision. 
	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f), true);
	AttackCollision->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->Activate(false);

	// Set SpringArm.
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//SpringArm->SetRelativeRotation(FRotator(-100.f, 0.f, 0.f));
	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->TargetArmLength = 1500;
	//SpringArm->bUsePawnControlRotation = false;
	//SpringArm->bInheritYaw = false;
	//SpringArm->bInheritPitch = false;
	//SpringArm->bInheritRoll = false;

	//// Set Camera.
	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// SpringEndpoint�� ī�޶� ����.
	//Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//Camera->bUsePawnControlRotation = false;

	// set mapping context. 
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Input/IMC_LOLVampireSurvivors.IMC_LOLVampireSurvivors"));
	if (InputMappingContextRef.Succeeded())
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	// set input action. 
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Input/Action/LOLVapireSurvivors/VSIA_Move.VSIA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Input/Action/LOLVapireSurvivors/VSIA_Look.VSIA_Look"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> DefaultAttackActionRef(TEXT("/Game/Input/Action/LOLVapireSurvivors/VSIA_SkillAttack.VSIA_SkillAttack"));
	if (DefaultAttackActionRef.Succeeded())
	{
		SkillAttackAction = DefaultAttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> UltimateSkillAttackActionRef(TEXT("/Game/Input/Action/LOLVapireSurvivors/VSIA_UltimateSkillAttack.VSIA_UltimateSkillAttack"));
	if (DefaultAttackActionRef.Succeeded())
	{
		UltimateSkillAttackAction = UltimateSkillAttackActionRef.Object;
	}

	// @NHRTodo: è�Ǿ� �� �������Ʈ�� ����� -> ������ �������� �����ϱ�.
	// set animation montage.
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> DefaultAttackMontageRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Champions/Rammus/Animations/VSAM_RammusDefaultAttack.VSAM_RammusDefaultAttack"));
	//if (DefaultAttackMontageRef.Succeeded())
	//{
	//	DefaultAttack = DefaultAttackMontageRef.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillAttackMontageRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Champions/Rammus/Animations/VSAM_RammusSkillAttack.VSAM_RammusSkillAttack"));
	//if (DefaultAttackMontageRef.Succeeded())
	//{
	//	SkillAttack = SkillAttackMontageRef.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> UltimateSkillAttackMontageRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Champions/Rammus/Animations/VSAM_RammusUltimateSkillAttack.VSAM_RammusUltimateSkillAttack"));
	//if (UltimateSkillAttackMontageRef.Succeeded())
	//{
	//	UltimateSkillAttack = UltimateSkillAttackMontageRef.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/BluePrints/LOLVampireSurvivors/Characters/Champions/Rammus/Animations/VSAM_RammusDead.VSAM_RammusDead"));
	//if (DeadMontageRef.Succeeded())
	//{
	//	Dead = DeadMontageRef.Object;
	//}

	//// set attack cool time.
	//SkillCoolTime = 10.f;
	//UltimateSkillCoolTime = 30.f;

	bGenerateOverlapEventsDuringLevelStreaming = true;

	HPLevel = 1;
	AttackRadiusLevel = 1;
	AttackPowerLevel = 1;

	bAttackDirty = true;
	bSkillAttackDirty = true;
	bUltimateSkillAttackDirty = true;
}

void AVSCharacterChampionPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVSCharacterChampionPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVSCharacterChampionPlayer::Look);
	//EnhancedInputComponent->BindAction(SkillAttackAction, ETriggerEvent::Triggered, this, &AVSCharacterChampionPlayer::SkillAttackBegin);
	//EnhancedInputComponent->BindAction(UltimateSkillAttackAction, ETriggerEvent::Triggered, this, &AVSCharacterChampionPlayer::UltimateSkillAttackBegin);

	SetupGASInputComponent();
}

void AVSCharacterChampionPlayer::OnLevelUp()
{
	LOL_LOG(LogProjectLOL, Warning, TEXT("Level Up"));

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	PlayerController->SetPause(true);
}

void AVSCharacterChampionPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(SkillAttackAction, ETriggerEvent::Triggered, this, &AVSCharacterChampionPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(SkillAttackAction, ETriggerEvent::Completed, this, &AVSCharacterChampionPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(UltimateSkillAttackAction, ETriggerEvent::Triggered, this, &AVSCharacterChampionPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(UltimateSkillAttackAction, ETriggerEvent::Completed, this, &AVSCharacterChampionPlayer::GASInputReleased, 1);
	}
}

void AVSCharacterChampionPlayer::GASInputPressed(int32 InputID)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("InputID : %d"), InputID);

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			// �Է� �Դٴ� ��ȣ ����.
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			// Ȱ��ȭ.
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AVSCharacterChampionPlayer::GASInputReleased(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			// �Է� ���� �ƴٴ� ��ȣ ����.
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AVSCharacterChampionPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(
				InputMappingContext,
				0
			);
		}
	}

	// check.
	check(GetMesh());
	check(ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance()));

	//// �ִϸ��̼� ��� �ð� ��������. 
	//DefaultAttackTime = DefaultAttackMontage->GetAnimationData(TEXT("DefaultAttack"))->GetLength();
	//SkillAttackTime = SkillAttackMontage->GetAnimationData(TEXT("SkillAttack"))->GetLength();
	//UltimateSkillAttackTime = UltimateSkillAttackMontage->GetAnimationData(TEXT("UltimateSkillAttack"))->GetLength();
	if (DeadMontage)
	{
		DeadTime = DeadMontage->GetPlayLength();
	}

}

void AVSCharacterChampionPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Controller)
	//{
	//	// @NHRTodo: ����ȭ. 
	//	// ���콺 Ŀ�� ��� ĳ���� ȸ��.
	//	FHitResult HitResult;
	//	Cast<APlayerController>(Controller)->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, 0, HitResult);
	//	FVector MouseDirection = HitResult.Location - Controller->GetPawn()->GetActorLocation();
	//	MouseDirection.Normalize();

	//	Controller->SetControlRotation(FRotationMatrix::MakeFromX(MouseDirection).Rotator());

	//	//static float TestAttackTimer;
	//	//TestAttackTimer += DeltaTime;
	//	//if (TestAttackTimer >= AttackTime)
	//	//{
	//	//	TestAttackTimer = 0;
	//	//	DefaultAttackBegin();
	//	//}
	//}
}

UAbilitySystemComponent* AVSCharacterChampionPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AVSCharacterChampionPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// PlayerState���� ASC ��������. 
	AVSPlayerState* VSPlayerState = GetPlayerState<AVSPlayerState>();
	if (VSPlayerState)
	{
		ASC = VSPlayerState->GetAbilitySystemComponent();

		// OwnerActor�� AvatarActor �ʱ�ȭ. 
		ASC->InitAbilityActorInfo(VSPlayerState, this);

		// Level Up ��������Ʈ ����.
		UVSChampionAttributeSet* CharacterAttributeSet = VSPlayerState->GetChampionAttributeSet();
		CharacterAttributeSet->OnLevelUp.AddDynamic(this, &AVSCharacterChampionPlayer::OnLevelUp);
		CharacterAttributeSet->OnOutOfHP.AddDynamic(this, &AVSCharacterChampionPlayer::DeadBegin);
		CharacterAttributeSet->SetMaxHP(CharacterAttributeSet->GetMaxHP() * 10.f);
		CharacterAttributeSet->InitHP(CharacterAttributeSet->GetMaxHP());

		if (HasAuthority())
		{
			// �ߵ��� GA �ο�.
			for (const auto& StartAbility : StartAbilites)
			{
				FGameplayAbilitySpec StartSpec(StartAbility);
				ASC->GiveAbility(StartSpec);
			}
			// �ߵ��� input GA �ο�.
			for (const auto& StartInputAbility : StartInputAbilities)
			{
				FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
				StartSpec.InputID = StartInputAbility.Key;
				ASC->GiveAbility(StartSpec);
			}
		}

		SetupGASInputComponent();

		// GameplayTag Debug.
		//APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		//PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

		//// GE ����.
		//FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		//EffectContextHandle.AddSourceObject(this);
		//FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitHPEffect, HPLevel, EffectContextHandle);
		//if (EffectSpecHandle.IsValid())
		//{
		//	ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		//}
		//
		//for (auto InitAttackRadiusEffect : InitAttackRadiusEffects)
		//{
		//	EffectSpecHandle = ASC->MakeOutgoingSpec(InitAttackRadiusEffect, AttackRadiusLevel, EffectContextHandle);
		//	if (EffectSpecHandle.IsValid())
		//	{
		//		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		//	}
		//}

		//for (auto InitAttackPowerEffect : InitAttackPowerEffects)
		//{
		//	EffectSpecHandle = ASC->MakeOutgoingSpec(InitAttackPowerEffect, AttackPowerLevel, EffectContextHandle);
		//	if (EffectSpecHandle.IsValid())
		//	{
		//		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		//	}
		//}

		if (HasAuthority())
		{
			// set attack timer. 
			AttackTimerHandle.Invalidate();	// init AttackTimer.

			// timer test.
			GetWorld()->GetTimerManager().SetTimer(
				AttackTimerHandle,
				FTimerDelegate::CreateLambda([&]()
					{
						AVSCharacterChampionPlayer::DefaultAttackBegin();
					}),
				AttackTime,
				true
			);
		}

		// @NHRTodo: �ӽ� ��ų GA �׽�Ʈ. 
		//for (const auto& TestAbility : TestAbilities)
		//{
		//	FGameplayAbilitySpec TestSpec(TestAbility);
		//	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(TestSpec);
		//	ASC->TryActivateAbility(Handle);
		//}

	}
}

void AVSCharacterChampionPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

}

void AVSCharacterChampionPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AVSCharacterChampionPlayer, AttackCollision);
}

void AVSCharacterChampionPlayer::Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		FVector2D Movement = Value.Get<FVector2D>();

		float MovementVectorSize = 1.0f;
		float MovementVectorSizeSquared = Movement.SizeSquared();

		if (MovementVectorSizeSquared > 1.0f)
		{
			Movement.Normalize();
			MovementVectorSizeSquared = 1.0f;
		}
		else
		{
			MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
		}

		FVector MoveDirection = FVector(Movement.X, Movement.Y, 0.0f);
		//GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

		AddMovementInput(MoveDirection, MovementVectorSize);
	}
}

void AVSCharacterChampionPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AVSCharacterChampionPlayer::DefaultAttackBegin()
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	ServerRPCDefaultAttack();

	//AttackCollision->Activate(true);

	////ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	//if (ChampionAnimInstance)
	//{
	//	// play montage.
	//	//RammusAnimInstance->SetOnAttack(true);
	//	ChampionAnimInstance->Montage_Play(DefaultAttack, 1.f);

	//	// bind delegate.
	//	FOnMontageEnded DefaultAttackEndDelegate;
	//	DefaultAttackEndDelegate.BindUObject(this, &AVSCharacterChampionPlayer::DefaultAttackEnd);
	//	ChampionAnimInstance->Montage_SetEndDelegate(DefaultAttackEndDelegate, DefaultAttack);

	//	UE_LOG(LogTemp, Log, TEXT("DefaultAttackBegin"));
	//}
}

void AVSCharacterChampionPlayer::DefaultAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//AttackCollision->Activate(false);
	////ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	//if (ChampionAnimInstance)
	//{
	//	ChampionAnimInstance->SetOnAttack(false);
	//	UE_LOG(LogTemp, Log, TEXT("DefaultAttackEnd"));
	//}

	// @NHRTodo: UI Delegate.
}

void AVSCharacterChampionPlayer::SkillAttackBegin()
{
	if (bSkillAttackDirty)
	{
		bSkillAttackDirty = false;
		ServerRPCSkillAttack();
	}

	//ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	//if (ChampionAnimInstance)
	//{
	//	ChampionAnimInstance->StopAllMontages(0.f);

	//	// play montage.
	//	ChampionAnimInstance->Montage_Play(SkillAttack, 1.f);

	//	// bind delegate.
	//	FOnMontageEnded SkillAttackEndDelegate;
	//	SkillAttackEndDelegate.BindUObject(this, &AVSCharacterChampionPlayer::SkillAttackEnd);
	//	ChampionAnimInstance->Montage_SetEndDelegate(SkillAttackEndDelegate, SkillAttack);

	//	UE_LOG(LogTemp, Log, TEXT("SkillAttackBegin"));
	//}
}

void AVSCharacterChampionPlayer::SkillAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	AttackCollision->Activate(false);
	//ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	if (ChampionAnimInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("SkillAttackEnd"));

		// @NHRTodo: UI Delegate.
	}
}

void AVSCharacterChampionPlayer::UltimateSkillAttackBegin()
{
	if (bUltimateSkillAttackDirty)
	{
		bUltimateSkillAttackDirty = false;
		ServerRPCUltimateSkillAttack();
	}

	//ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	//if (ChampionAnimInstance)
	//{
	//	ChampionAnimInstance->StopAllMontages(0.f);

	//	// play montage.
	//	ChampionAnimInstance->Montage_Play(UltimateSkillAttack, 1.f);

	//	// bind delegate.
	//	FOnMontageEnded UltimateSkillAttackEndDelegate;
	//	UltimateSkillAttackEndDelegate.BindUObject(this, &AVSCharacterChampionPlayer::UltimateSkillAttackEnd);
	//	ChampionAnimInstance->Montage_SetEndDelegate(UltimateSkillAttackEndDelegate, UltimateSkillAttack);

	//	UE_LOG(LogTemp, Log, TEXT("UltimateSkillAttackBegin"));
	//}
}

void AVSCharacterChampionPlayer::UltimateSkillAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	if (ChampionAnimInstance)
	{
		LOL_LOG(LogProjectLOL, Log, TEXT("UltimateSkillAttackEnd"));

		// @NHRTodo: UI Delegate.
	}
}

void AVSCharacterChampionPlayer::DeadBegin()
{
	LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
	//ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	if (ChampionAnimInstance)
	{
		ChampionAnimInstance->StopAllMontages(0.f);

		// play montage.
		ChampionAnimInstance->Montage_Play(DeadMontage, 1.f);

		// bind delegate.
		FOnMontageEnded DeadDelegate;
		DeadDelegate.BindUObject(this, &AVSCharacterChampionPlayer::DeadEnd);
		ChampionAnimInstance->Montage_SetEndDelegate(DeadDelegate, DeadMontage);
	}
}

void AVSCharacterChampionPlayer::DeadEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	ASC->CancelAbilities();
	ASC->ClearAllAbilities();

	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
	SetActorHiddenInGame(true);

	GetWorld()->GetSubsystem<UVSGameSubsystem>()->OnGameOverDelegate.Broadcast();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	PlayerController->SetPause(true);

}

bool AVSCharacterChampionPlayer::ServerRPCDefaultAttack_Validate()
{
	return true;
}

void AVSCharacterChampionPlayer::ServerRPCDefaultAttack_Implementation()
{
	LOL_LOG(LogProjectLOL, Log, TEXT("ServerRPCDefaultAttack_Implementation"));

	MulticastRPCDefaultAttack();
}

void AVSCharacterChampionPlayer::MulticastRPCDefaultAttack_Implementation()
{
	LOL_LOG(LogProjectLOL, Log, TEXT("MulticastRPCDefaultAttack_Implementation"));

	if (HasAuthority())
	{
		AttackCollision->Activate(true);

		// NHRTodo: ���߿� GA �ȿ��� Loop��.
		FGameplayAbilitySpec StartSpec(StartAbilites[0]);
		FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(StartSpec);

		ASC->TryActivateAbility(Handle);
	}

	//if (bAttackDirty)
	//{
	//	FGameplayAbilitySpec StartSpec(StartAbilites[0]);
	//	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(StartSpec);

	//	ASC->TryActivateAbility(Handle);

	//	bAttackDirty = false;
	//}

	//ASC->TryActivateAbilityByClass(UVSGA_Attack::StaticClass());

	// �ִϸ��̼� ���.
	//ChampionAnimInstance = Cast<UVSChampionAnimInstance>(GetMesh()->GetAnimInstance());
	//if (ChampionAnimInstance)
	//{
	//	// play montage.
	//	//RammusAnimInstance->SetOnAttack(true);
	//	//ChampionAnimInstance->Montage_Play(DefaultAttackMontage, 1.f);

	//	//FTimerHandle AnimationTimerHandle;
	//	//GetWorld()->GetTimerManager().SetTimer(
	//	//	AnimationTimerHandle,
	//	//	FTimerDelegate::CreateLambda([&]()
	//	//		{
	//	//			if (HasAuthority())
	//	//			{
	//	//				AttackCollision->Activate(false);
	//	//			}

	//	//			// �ִϸ��̼� ����.
	//	//			if (ChampionAnimInstance)
	//	//			{
	//	//				ChampionAnimInstance->SetOnAttack(false);
	//	//				UE_LOG(LogTemp, Log, TEXT("DefaultAttackEnd"));

	//	//			}

	//	//		}), DefaultAttackTime, false);

	//	// bind delegate.
	//	//FOnMontageEnded DefaultAttackEndDelegate;
	//	//DefaultAttackEndDelegate.BindUObject(this, &AVSCharacterChampionPlayer::DefaultAttackEnd);
	//	//ChampionAnimInstance->Montage_SetEndDelegate(DefaultAttackEndDelegate, DefaultAttack);

	//	//UE_LOG(LogTemp, Log, TEXT("DefaultAttackBegin"));
	//}

}

bool AVSCharacterChampionPlayer::ServerRPCSkillAttack_Validate()
{
	return true;
}

void AVSCharacterChampionPlayer::ServerRPCSkillAttack_Implementation()
{
	MulticastRPCSkillAttack();
}

void AVSCharacterChampionPlayer::MulticastRPCSkillAttack_Implementation()
{
	if (HasAuthority())
	{
		AttackCollision->Activate(false);
	}

	//if (ChampionAnimInstance)
	//{
	//	//ChampionAnimInstance->StopAllMontages(0.f);

	//	// play montage.
	//	//ChampionAnimInstance->Montage_Play(SkillAttackMontage, 1.f);

	//	//FTimerHandle AnimationTimerHandle;
	//	//GetWorld()->GetTimerManager().SetTimer(
	//	//	AnimationTimerHandle,
	//	//	FTimerDelegate::CreateLambda([&]()
	//	//		{
	//	//			// �ִϸ��̼� ����.
	//	//			if (ChampionAnimInstance)
	//	//			{
	//	//				UE_LOG(LogTemp, Log, TEXT("SkillAttackCoolEnd"));
	//	//				
	//	//				// NHRTodo: ��Ÿ�� ���� �ʿ�.
	//	//				bSkillAttackDirty = true;
	//	//			}

	//	//		}), SkillAttackTime + SkillCoolTime, false);

	//	//// bind delegate.
	//	//FOnMontageEnded SkillAttackEndDelegate;
	//	//SkillAttackEndDelegate.BindUObject(this, &AVSCharacterChampionPlayer::SkillAttackEnd);
	//	//ChampionAnimInstance->Montage_SetEndDelegate(SkillAttackEndDelegate, SkillAttack);

	//	//UE_LOG(LogTemp, Log, TEXT("SkillAttackBegin"));
	//}
}

bool AVSCharacterChampionPlayer::ServerRPCUltimateSkillAttack_Validate()
{
	return true;
}

void AVSCharacterChampionPlayer::ServerRPCUltimateSkillAttack_Implementation()
{
	MulticastRPCUltimateSkillAttack();
}

void AVSCharacterChampionPlayer::MulticastRPCUltimateSkillAttack_Implementation()
{
	if (ChampionAnimInstance)
	{
		ChampionAnimInstance->StopAllMontages(0.f);

		// play montage.
		//ChampionAnimInstance->Montage_Play(UltimateSkillAttackMontage, 1.f);

		//FTimerHandle AnimationTimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(
		//	AnimationTimerHandle,
		//	FTimerDelegate::CreateLambda([&]()
		//		{
		//			// �ִϸ��̼� ����.
		//			if (ChampionAnimInstance)
		//			{
		//				UE_LOG(LogTemp, Log, TEXT("UltimateSkillAttackEnd"));

		//				// NHRTodo: ��Ÿ�� ���� �ʿ�.
		//				bUltimateSkillAttackDirty = true;
		//			}

		//		}), UltimateSkillAttackTime + UltimateSkillCoolTime, false);

		//// bind delegate.
		//FOnMontageEnded UltimateSkillAttackEndDelegate;
		//UltimateSkillAttackEndDelegate.BindUObject(this, &AVSCharacterChampionPlayer::UltimateSkillAttackEnd);
		//ChampionAnimInstance->Montage_SetEndDelegate(UltimateSkillAttackEndDelegate, UltimateSkillAttack);

		UE_LOG(LogTemp, Log, TEXT("UltimateSkillAttackBegin"));
	}
}

void AVSCharacterChampionPlayer::OnSkill(FName SelectedGAName)
{
	if (HasAuthority())
	{
		// ��ųGA ��Ͽ� ���Ե� GA���� �˻�.
		if (!UnionSkillAbilities.Contains(SelectedGAName))
		{
			LOL_LOG(LogProjectLOL, Error, TEXT("NOT INCLUDED GA"));

			return;
		}

		TSubclassOf<UGameplayAbility> SelectedGA = UnionSkillAbilities[SelectedGAName];

		// �̹� �ߵ����� GA���� �˻�.
		for (const FGameplayAbilitySpec& HasSpec : ASC->GetActivatableAbilities())
		{
			if (HasSpec.Ability && HasSpec.Ability->GetClass() == SelectedGA)
			{
				LOL_LOG(LogProjectLOL, Warning, TEXT("ALREADY HAVE GA"));

				// ���׷��̵�.
				UpgrateGA(SelectedGAName);

				return;
			}
		}

		// GA �ο�.
		FGameplayAbilitySpec Spec(SelectedGA);
		FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
		ASC->TryActivateAbility(Handle);
	}
}

// NHRTodo: ���� �����ϱ�. 
void AVSCharacterChampionPlayer::UpgrateGA(FName SelectedGAName)
{
	if (HasAuthority())
	{
		// PlayerState�� AttributeSet ��������.
		AVSPlayerState* VSPlayerState = GetPlayerState<AVSPlayerState>();
		if (VSPlayerState)
		{
			UVSChampionAttributeSet* AttributeSet = VSPlayerState->GetChampionAttributeSet();

			if (SelectedGAName == "BPGA_UnionSkillFireBall")
			{
				LOL_LOG(LogProjectLOL, Log, TEXT("FIREBALL UPGRADE"));
				AttributeSet->SetFireballSkillLevel((AttributeSet->GetFireballSkillLevel() + 0.5f));
			}
			else if (SelectedGAName == "BPGA_UnionSkillShield")
			{
				LOL_LOG(LogProjectLOL, Log, TEXT("SHIELD UPGRADE"));
				AttributeSet->SetShieldSkillLevel((AttributeSet->GetShieldSkillLevel() + 0.5f));
			}
			else if (SelectedGAName == "BPGA_UnionSkillLaser")
			{
				LOL_LOG(LogProjectLOL, Log, TEXT("LASER UPGRADE"));
				AttributeSet->SetLaserSkillLevel((AttributeSet->GetLaserSkillLevel() + 0.5f));
			}
		}
	}
}
