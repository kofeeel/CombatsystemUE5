// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSCharacter.generated.h"

class ACharacter;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class ACSWeapon;
class ACSShield;
class ACSRangedWeapon;

class UCSHealthComponent;
class UCSStaminaComponent;
class UCSCameraManagerComponent;

class UCSCharacterState;
enum class CharacterStateType : uint8;

class UNiagaraSystem;

// 상태 변경 시 호출되는 델리게이트
DECLARE_DELEGATE_OneParam(CSStateDelegate, CharacterStateType);
DECLARE_DELEGATE_ThreeParams(CSStateKeyDelegate, CharacterStateType, FString, EInputEvent);

// 전투 유형 열거형
enum class CSCombatType : uint8
{
	MELEE,   // 근접 전투
	RANGED   // 원거리 전투
};

UCLASS()
class COMBATSYSTEM_API ACSCharacter : public ACharacter
{
	GENERATED_BODY()


protected:
	// 캐릭터의 기본 값을 설정합니다.
	ACSCharacter();

	// 게임이 시작되거나 스폰될 때 호출됩니다.
	virtual void BeginPlay() override;

	// 캐릭터가 파괴될 때 사용할 Niagara 시스템
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "CSCharacter")
	UNiagaraSystem* DestroyNiagaraSystem;

	// 이동 함수
	void MoveForward(float Value);
	void MoveRight(float Value);

	// 게임패드용 회전 및 보기 함수
	void Turn(float Value);
	void LookUp(float Value);

	// 마우스용 회전 및 보기 속도 함수
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	float TurnRate;
	float LookRate;

	// 달리기 시작 함수
	void StartRunning();

	UPROPERTY(BlueprintReadOnly, Category = "CSCharacter")
	bool CanMove;

	// 캐릭터 속도 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Player")
	float JogSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Player")
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Player")
	float LockedSpeed;

	// 컴포넌트 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Components")
	UCSCameraManagerComponent* CameraManagerComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Components")
	UCSHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Components")
	UCSStaminaComponent* StaminaComp;

	// 체력 변화 시 호출되는 함수
	UFUNCTION()
	void OnHealthChanged(UCSHealthComponent* HealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// 타겟 잠금 관련 변수 및 함수 ======================================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	bool TargetLocked;

	ACSCharacter* LockedEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float TimeBetweenEnemyChange;

	bool CanChangeLockedEnemy;
	void ToggleLockTarget();
	bool LockTarget();
	void ChangeLockedTarget(float Direction);
	void EnableLockedEnemyChange();
	void UnlockTarget();

	// 적 탐지 관련 변수 및 함수 ======================================================================================
	TArray<ACSCharacter*> GetAllVisibleEnemies(float Radius);

	bool IsEnemyVisible(ACSCharacter* Enemy);

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float EnemyDetectionDistance;

	void OnDetectNearbyEnemies();

	TArray<ACharacter*> NearbyEnemies;

	// 상태 관련 변수 및 함수 ======================================================================================
	UPROPERTY(EditDefaultsOnly)
	float RequestTime;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UCSCharacterState>> DefaultStates;

	UPROPERTY(BlueprintReadOnly)
	TMap<CharacterStateType, UCSCharacterState*> States;

	UPROPERTY(BlueprintReadonly)
	CharacterStateType CurrentState;

	void AddState(TSubclassOf<UCSCharacterState> ActionClass);

	UFUNCTION(BlueprintCallable)
	void OnAnimationEnded(CharacterStateType FinishedAnimationState);

	UFUNCTION(BlueprintCallable)
	void OnAnimationNotify(CharacterStateType StateType, FString AnimationNotifyName);

	void NotifyActionToState(CharacterStateType StateType, FString ActionName, EInputEvent KeyEvent);

	bool Parriable;

	void SpawnEquipment();

	// 무기 관련 변수 및 함수 ==========================================================================================
	ACSWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ACSWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	// 방패 관련 변수 및 함수 ==========================================================================================
	ACSShield* CurrentShield;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ACSShield> StarterShieldClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName ShieldAttachSocketName;

	// 원거리 무기 관련 변수 및 함수 ===================================================================================
	ACSRangedWeapon* CurrentRangedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ACSRangedWeapon> StarterRangedWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName RangedWeaponAttachSocketName;

	CSCombatType CurrentCombatType;

public:
	// 함수 정의 ============================================================================================= 

	// 매 프레임마다 호출됩니다.
	virtual void Tick(float DeltaTime) override;

	// 입력에 기능을 바인딩합니다.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCanMove(bool NewCanMove);

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable)
	UCSHealthComponent* GetHealthComponent() const;

	UFUNCTION(BlueprintCallable)
	UCSStaminaComponent* GetStaminaComponent() const;

	UFUNCTION(BlueprintCallable)
	ACSWeapon* GetCurrentWeapon();

	ACSRangedWeapon* GetCurrentRangedWeapon() const;

	bool IsTargetLocked() const;

	UFUNCTION(BlueprintCallable)
	bool IsStateRequested(CharacterStateType Type);

	UFUNCTION(BlueprintCallable)
	UCSCharacterState* GetCharacterState(CharacterStateType StateType);

	UFUNCTION(BlueprintCallable)
	ACSCharacter* GetLockedTarget() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetAsTarget(bool IsTarget);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit();

	void ChangeState(CharacterStateType NewState, uint8 NewSubstate = 0u);

	UFUNCTION(BlueprintCallable)
	void RequestState(CharacterStateType Type);

	void RequestStateAndSubstate(CharacterStateType StateType, uint8 CurrentSubstate);

	CharacterStateType GetCurrentState() const;

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentSubstate() const;

	UFUNCTION(BlueprintCallable)
	uint8 GetStateCurrentSubstate(CharacterStateType StateType) const;

	UFUNCTION(BlueprintCallable)
	void SetParriable(bool Parriable);

	bool IsParriable() const;

	void OnEnemyDead(ACSCharacter* DeadCharacter);

	bool IsFacingActor(AActor* OtherActor, float AngleThreshold = 150.0f);

	TArray<ACharacter*> GetNearbyEnemies() const;

	UFUNCTION(BlueprintImplementableEvent)
	void SetCrosshairActive(bool Active);

	void ChangeCombatType(CSCombatType NewCombatType);

	UCSCameraManagerComponent* GetCameraManager() const;

	void PlayForceFeedback(UForceFeedbackEffect* ForceFeedback, FForceFeedbackParameters ForceFeedbackParameters = FForceFeedbackParameters());
	void StopForceFeedback(UForceFeedbackEffect* ForceFeedback);

	float GetMovementSpeed() const;
	void SetMaxWalkSpeed(float NewMaxWalkSpeed);
	void ResetMaxWalkSpeed();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(float UpdatedHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStamina(float UpdatedStamina);

	UFUNCTION(BlueprintCallable)
	FRotator GetAimRotation();

	// 변수 정의 =============================================================================================
	UPROPERTY(BlueprintReadonly)
	bool IsRunning;

	void StopRunning();

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UCameraComponent* CameraComp;

	CharacterStateType LastState;

	float MaxDistanceToEnemies;

	void StartDestroy();
};
