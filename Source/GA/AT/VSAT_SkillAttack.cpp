// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/VSAT_SkillAttack.h"
#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"

UVSAT_SkillAttack::UVSAT_SkillAttack()
{
}

UVSAT_SkillAttack* UVSAT_SkillAttack::CreateTask(UGameplayAbility* OwningAbility)
{
    // AT �ν��Ͻ� ����. 
    UVSAT_SkillAttack* NewTask = NewAbilityTask<UVSAT_SkillAttack>(OwningAbility);
    return NewTask;
}

void UVSAT_SkillAttack::Activate()
{
    Super::Activate();

    // @NHRTodo : Skill Attack ���� �߰��ϱ�. Collision, Effect ��.
    //AVSCharacterChampionPlayer* VSCharacterChampion = CastChecked<AVSCharacterChampionPlayer>(GetAvatarActor());

    // ���� �ð� ��⸦ ���� Waiting ���� ȣ��.
    SetWaitingOnAvatar();
}

void UVSAT_SkillAttack::OnDestroy(bool AbilityEnded)
{
    Super::OnDestroy(AbilityEnded);
}
