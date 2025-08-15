// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LOLVampireSurvivors/VSCharacterBase.h"

#include "InputActionValue.h"

#include "AbilitySystemInterface.h"

#include "VSCharacterChampionPlayer.generated.h"

class UInputAction;
class UInputMappingContext;
class USkeletalMesh;
class UBoxComponent;
class UAnimationAsset;
class UAnimMontage;
class UVSChampionAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTLOL_API AVSCharacterChampionPlayer : public AVSCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	explicit AVSCharacterChampionPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//FORCEINLINE UAnimMontage* GetAttackMontage() { return DefaultAttackMontage; };
	//FORCEINLINE UAnimMontage* GetSkillAttackMontage() { return SkillAttackMontage; };
	//FORCEINLINE UAnimMontage* GetUltimateSkillAttackMontage() { return UltimateSkillAttackMontage; };

	FORCEINLINE class UVSAttackDataAsset* GetAttackData() { return AttackData; }

protected:

	// Transform. 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	//void Rotate(const FInputActionValue& Value);

	// Attack.
	void DefaultAttackBegin();			// Auto.
	void DefaultAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SkillAttackBegin();				// E key.
	void SkillAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void UltimateSkillAttackBegin();		// R key. 
	void UltimateSkillAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// Behavior.
	UFUNCTION()
	void DeadBegin();
	
	UFUNCTION()
	void DeadEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// LevelUp.
	UFUNCTION()
	void OnLevelUp();

	// GAS.
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputID);
	void GASInputReleased(int32 InputID);

	void UpgrateGA(FName SelectedGAName);

public:
	UFUNCTION(BlueprintCallable)
	void OnSkill(FName SelectedGAName);

// Champion.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Champion, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<USkeletalMesh> ChampionSkeletalMesh;
	
// Input.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPricateAccess = "true"))
	//TObjectPtr<class UInputAction> CursorAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UInputAction> SkillAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UInputAction> UltimateSkillAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

// Attack.
protected:
	FTimerHandle AttackTimerHandle;

	// NHRTodo: 임시 데이터. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	float AttackTime = 2.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	TObjectPtr<UBoxComponent> AttackCollision;

// Animation.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Champion, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UAnimationAsset> ChampionAnimationAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UVSChampionAnimInstance> ChampionAnimInstance;

	////Default Attack.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//TObjectPtr<UAnimMontage> DefaultAttackMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//float DefaultAttackTime;

	//// Skill Attack. 
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//TObjectPtr<UAnimMontage> SkillAttackMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//float SkillAttackTime;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//float SkillCoolTime;

	//// Ultimate Skill Attack.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//TObjectPtr<UAnimMontage> UltimateSkillAttackMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//float UltimateSkillAttackTime;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	//float UltimateSkillCoolTime;

	// Dead.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPricateAccess = "true"))
	TObjectPtr<UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPricateAccess = "true"))
	float DeadTime;

// Network.
protected:
	
	// Default Attack.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCDefaultAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCDefaultAttack();

	// Skill Attack.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSkillAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSkillAttack();

	// Ultimate Skill Attack.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCUltimateSkillAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUltimateSkillAttack();

// UI.

// GAS.
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 시작 시 발동할 GA 목록.
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;
	
	// 시작 시 발동할 input GA 목록.
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	// 임시 테스트 용 GA.
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<FName, TSubclassOf<class UGameplayAbility>> UnionSkillAbilities;

protected:
	UPROPERTY(EditAnywhere, Category = Data)
	TObjectPtr<UVSAttackDataAsset> AttackData;

	UPROPERTY(EditAnywhere, Category = GAS)
	float HPLevel;

	UPROPERTY(EditAnywhere, Category = GAS)
	float AttackRadiusLevel;

	UPROPERTY(EditAnywhere, Category = GAS)
	float AttackPowerLevel;

// Dirty check.
public:
	uint8 bAttackDirty : 1;
	uint8 bSkillAttackDirty : 1;
	uint8 bUltimateSkillAttackDirty : 1;
};
