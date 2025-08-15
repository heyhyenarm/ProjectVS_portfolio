// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/LOLVampireSurvivors/Enemy/VSBoidCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "VSEnemyTrashMob.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UAnimInstance;
UCLASS()
class PROJECTLOL_API AVSEnemyTrashMob : public AVSBoidCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit AVSEnemyTrashMob();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE USphereComponent* GetSphereComponent() { return AttackCollision; };
	FORCEINLINE UAnimMontage* GetDeadMontage() { return DeadMontage; };
	FORCEINLINE UAnimMontage* GetAttackMontage() { return AttackMontage; };

	void Initialize();

protected:
	void Attack();
	void Dead();
	AActor OnActorBeginOverlap_Delegate(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	virtual void OnOutOfHP();

// Enemy.
protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enemy, meta = (AllowPricateAccess = "true"))
	//TObjectPtr<USkeletalMeshComponent> TrashMobBody;

// Attack.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPricateAccess = "true"))
	TObjectPtr<USphereComponent> AttackCollision;

// Animation.
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPricateAccess = "true"))
	//TObjectPtr<UAnimInstance> TrashMobAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	float AttackCoolTime;

// GAS.
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	// 시작 시 발동할 GA 목록.
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayAbility> TriggerAttackAbility;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UVSCharacterAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = GAS)
	FGameplayAbilitySpecHandle AttackHandle;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> DamageEffect;

// Dirty.
protected:
	UPROPERTY(VisibleAnywhere, Category = State)
	uint8 bOnOverlap : 1;

	UPROPERTY(VisibleAnywhere, Category = State)
	uint8 bOnDead : 1;
};
