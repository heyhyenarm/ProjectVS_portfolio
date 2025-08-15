// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "VSWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API UVSWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;

};
