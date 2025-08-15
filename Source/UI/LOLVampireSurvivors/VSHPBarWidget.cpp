// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LOLVampireSurvivors/VSHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Character/LOLVampireSurvivors/Champion/VSCharacterChampionPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/VSChampionAttributeSet.h"

#include "ProjectLOL.h"

UVSHPBarWidget::UVSHPBarWidget(const FObjectInitializer& ObjectInitializer)
	: IsHPBar(false)
{
	MaxHp = -1.0f;
}

void UVSHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	if (IsValid(HpProgressBar))
	{
		IsHPBar = true;
		ensure(HpProgressBar);
		HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
		ensure(HpStat);
	}
	else
	{
		IsHPBar = false;

		ExpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbExpBar")));
		ensure(ExpProgressBar);
		ExpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtExpStat")));
		ensure(ExpStat);
	}

	AVSCharacterChampionPlayer* PlayerCharacter = Cast<AVSCharacterChampionPlayer>(GetOwningPlayer()->GetPawn());
	if (IsValid(PlayerCharacter) && PlayerCharacter->HasAuthority())
	{
		const UVSChampionAttributeSet* AttributeSet = Cast<UVSChampionAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(UVSChampionAttributeSet::StaticClass()));
		if (IsHPBar)
		{
			AttributeSet->OnHPChanged.AddDynamic(this, &UVSHPBarWidget::UpdateHpBar);
			CurrentHp = AttributeSet->GetMaxHP();
			MaxHp = AttributeSet->GetMaxHP();

			UpdateHpBar(CurrentHp);
		}
		else
		{
			AttributeSet->OnExpChanged.AddDynamic(this, &UVSHPBarWidget::UpdateExpBar);
			CurrentExp = 0.f;
			MaxExp = AttributeSet->GetMaxTotalExp();

			UpdateExpBar(CurrentExp, MaxExp);
		}

	}
}

void UVSHPBarWidget::UpdateMaxHP(const float BaseMaxHP, const float ModifierMaxHP)
{
	MaxHp = BaseMaxHP + ModifierMaxHP;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UVSHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	//LOL_LOG(LogProjectLOL, Warning, TEXT("NewCurrentHp : % f"), NewCurrentHp);

	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UVSHPBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}

void UVSHPBarWidget::UpdateMaxExp(const float BaseMaxExp, const float ModifierMaxExp)
{
	MaxExp = BaseMaxExp + ModifierMaxExp;

	if (ExpProgressBar)
	{
		ExpProgressBar->SetPercent(CurrentExp / MaxExp);
	}

	if (ExpStat)
	{
		ExpStat->SetText(FText::FromString(GetExpStatText()));
	}
}

void UVSHPBarWidget::UpdateExpBar(float NewCurrentExp, float NewMaxExp)
{
	//LOL_LOG(LogProjectLOL, Warning, TEXT("NewCurrentExp : % f, NewMaxExp : %f"), NewCurrentExp, NewMaxExp);

	CurrentExp = NewCurrentExp;
	MaxExp = NewMaxExp;

	ensure(MaxExp > 0.0f);
	if (ExpProgressBar)
	{
		ExpProgressBar->SetPercent(CurrentExp / MaxExp);
	}

	if (ExpStat)
	{
		ExpStat->SetText(FText::FromString(GetExpStatText()));
	}
}

FString UVSHPBarWidget::GetExpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentExp, MaxExp);
}
