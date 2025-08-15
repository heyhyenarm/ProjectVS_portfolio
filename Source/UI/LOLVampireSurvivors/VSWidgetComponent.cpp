// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LOLVampireSurvivors/VSWidgetComponent.h"
#include "VSWidgetComponent.h"

#include "UI/LOLVampireSurvivors/VSUserWidget.h"


void UVSWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UVSUserWidget* VSUserWidget = Cast<UVSUserWidget>(GetWidget());
	if (VSUserWidget)
	{
		VSUserWidget->SetOwningActor(GetOwner());
	}
}
