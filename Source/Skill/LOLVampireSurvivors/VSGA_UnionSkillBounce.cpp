// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/LOLVampireSurvivors/VSGA_UnionSkillBounce.h"
#include "ProjectLOL.h"
#include "Kismet/GameplayStatics.h"

UVSGA_UnionSkillBounce::UVSGA_UnionSkillBounce()
{
    // GA의 인스턴싱 정책 지정.
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

	// NHTTodo : 강제 실행 중.
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

	//// 기존 인스턴스 제거
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

    //// 월드 위치를 화면 좌표로 변환하기.
    //bool bOnScreen = PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPos);

    //if (bOnScreen)
    //{
    //    // 화면 크기 구하기.
    //    FVector2D ViewportSize;
    //    GEngine->GameViewport->GetViewportSize(ViewportSize);

    //    bool bHitEdge = false;
    //    FVector CurrentVelocity = /* 캐릭터나 투사체 현재 이동 방향/속도 벡터 가져오기 */;

    //    // 좌우 화면 경계 충돌 체크.
    //    if (ScreenPos.X <= 0.f || ScreenPos.X >= ViewportSize.X)
    //    {
    //        CurrentVelocity.X = -CurrentVelocity.X;  // X 방향 반전
    //        bHitEdge = true;
    //    }

    //    // 상하 화면 경계 충돌 체크.
    //    if (ScreenPos.Y <= 0.f || ScreenPos.Y >= ViewportSize.Y)
    //    {
    //        CurrentVelocity.Y = -CurrentVelocity.Y;  // Y 방향 반전
    //        bHitEdge = true;
    //    }

    //    if (bHitEdge)
    //    {
    //        // 월드 좌표 방향 반영 (예: 이동 컴포넌트에 velocity 변경)
    //        // 이 부분은 캐릭터/투사체 이동 로직에 맞게 구현 필요
    //        //UpdateCharacterVelocity(CurrentVelocity);
    //    }
    //}
}
