// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/VSAT_SkillAttack.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"

UVSAT_SkillAttack::UVSAT_SkillAttack()
{
}

UVSAT_SkillAttack* UVSAT_SkillAttack::CreateTask(UGameplayAbility* OwningAbility)
{
    // AT 인스턴스 생성. 
    UVSAT_SkillAttack* NewTask = NewAbilityTask<UVSAT_SkillAttack>(OwningAbility);
    return NewTask;
}

void UVSAT_SkillAttack::Activate()
{
    Super::Activate();

    // @NHRTodo : Skill Attack 내용 추가하기. Collision, Effect 등.
    //AVSCharacterChampionPlayer* VSCharacterChampion = CastChecked<AVSCharacterChampionPlayer>(GetAvatarActor());

    // 일정 시간 대기를 위해 Waiting 상태 호출.
    SetWaitingOnAvatar();
}

void UVSAT_SkillAttack::OnDestroy(bool AbilityEnded)
{
    Super::OnDestroy(AbilityEnded);
}
