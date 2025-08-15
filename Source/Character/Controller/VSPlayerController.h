// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLOL_API AVSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	explicit AVSPlayerController();

protected:
	virtual void BeginPlay() override;

// HUD Section.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UVSHUDWidget> VSHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UVSHUDWidget> VSHUDWidget;

};
