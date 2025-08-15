// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSGA_UnionSkillBounce.h"
#include "ProjectLOL.h"
#include "Kismet/GameplayStatics.h"

UVSGA_UnionSkillBounce::UVSGA_UnionSkillBounce()
{
    // GA�� �ν��Ͻ� ��å ����.
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UVSGA_UnionSkillBounce::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	//bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	//if (!bResult)
	//{
	//	return false;
	//}

	//const AVSCharacterChampionPlayer* ChampionPlayer = Cast<AVSCharacterChampionPlayer>(ActorInfo->AvatarActor.Get());

	//return ChampionPlayer->bAttackDirty;

	// NHTTodo : ���� ���� ��.
	return true;
}

void UVSGA_UnionSkillBounce::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (AActor* Avatar = GetAvatarActorFromActorInfo())
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            NiagaraSystem,
            Avatar->GetActorLocation(),
            Avatar->GetActorRotation()
        );
    }
    GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UVSGA_UnionSkillBounce::OnTick, 0.016f, true);
}

void UVSGA_UnionSkillBounce::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//LOL_LOG(LogProjectLOL, Log, TEXT("Begin"));
}

void UVSGA_UnionSkillBounce::SpawnAttackActor(FVector Location)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar || !AttackActorClass) return;

	//// ���� �ν��Ͻ� ����
	//if (SpawnAttackActorInstance)
	//{
	//	//SpawnAttackActorInstance->Destroy();
 //       ObjectPool->ReturnToPool(SpawnAttackActorInstance);
	//}

	//FActorSpawnParameters Params;
	//Params.Owner = Avatar;
	//Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//AActor* AttackActor = GetWorld()->SpawnActor<AActor>(AttackActorClass, Location, FRotator::ZeroRotator, Params);
	//if (AttackActor)
	//{
	//	AttackActor->AttachToActor(Avatar, FAttachmentTransformRules::KeepWorldTransform);
	//	SpawnAttackActorInstance = AttackActor;
	//	SpawnAttackActorInstance->SetOwner(Avatar);
	//}
}

void UVSGA_UnionSkillBounce::OnTick()
{
    //AActor* AvatarActor = GetAvatarActorFromActorInfo();
    //
    //if (!AvatarActor)
    //{
    //    return;
    //}

    //APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    //if (!PC)
    //    return;

    //FVector2D ScreenPos;
    //FVector WorldLocation = AvatarActor->GetActorLocation();

    //// ���� ��ġ�� ȭ�� ��ǥ�� ��ȯ�ϱ�.
    //bool bOnScreen = PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPos);

    //if (bOnScreen)
    //{
    //    // ȭ�� ũ�� ���ϱ�.
    //    FVector2D ViewportSize;
    //    GEngine->GameViewport->GetViewportSize(ViewportSize);

    //    bool bHitEdge = false;
    //    FVector CurrentVelocity = /* ĳ���ͳ� ����ü ���� �̵� ����/�ӵ� ���� �������� */;

    //    // �¿� ȭ�� ��� �浹 üũ.
    //    if (ScreenPos.X <= 0.f || ScreenPos.X >= ViewportSize.X)
    //    {
    //        CurrentVelocity.X = -CurrentVelocity.X;  // X ���� ����
    //        bHitEdge = true;
    //    }

    //    // ���� ȭ�� ��� �浹 üũ.
    //    if (ScreenPos.Y <= 0.f || ScreenPos.Y >= ViewportSize.Y)
    //    {
    //        CurrentVelocity.Y = -CurrentVelocity.Y;  // Y ���� ����
    //        bHitEdge = true;
    //    }

    //    if (bHitEdge)
    //    {
    //        // ���� ��ǥ ���� �ݿ� (��: �̵� ������Ʈ�� velocity ����)
    //        // �� �κ��� ĳ����/����ü �̵� ������ �°� ���� �ʿ�
    //        //UpdateCharacterVelocity(CurrentVelocity);
    //    }
    //}
}
