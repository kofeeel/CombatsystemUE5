// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSProjectile.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class COMBATSYSTEM_API ACSProjectile : public AActor
{
	GENERATED_BODY()

public:
	// 이 액터의 기본 값을 설정합니다.
	ACSProjectile();

protected:
	// 게임이 시작되거나 스폰될 때 호출됩니다.
	virtual void BeginPlay() override;

	// 충돌 컴포넌트 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComp;

	// 메쉬 컴포넌트 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// 오버랩 이벤트 처리 함수
	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 투사체의 기본 피해량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float BaseDamage;

	// 투사체의 피해 유형
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UDamageType> DamageType;

	// 투사체의 트레일 효과
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraSystem* TrailEffect;

	// 트레일 컴포넌트
	UNiagaraComponent* TrailComponent;

	// 충돌 효과 재생 함수
	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	// 기본 충돌 효과
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraSystem* DefaultImpactEffect;

	// 육체 충돌 효과
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	UNiagaraSystem* FleshImpactEffect;

	// 기본 충돌 소리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sounds")
	USoundBase* DefaultImpactSound;

	// 육체 충돌 소리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Sounds")
	USoundBase* FleshImpactSound;

	// 피해 배수
	float DamageMultiplier;

	// 파괴 가능 여부
	bool CanBeDestroyed;

	// 파괴 가능 여부 설정 함수
	void SetCanBeDestroyed();

	// 충돌한 물리적 표면 감지 함수
	EPhysicalSurface DetectCollidedPhysicalSurface();

public:
	// 매 프레임마다 호출됩니다.
	virtual void Tick(float DeltaTime) override;

	// 메쉬 컴포넌트 반환 함수
	UStaticMeshComponent* GetMesh();

	// 피해 배수 설정 함수
	void SetDamageMultiplier(float NewDamageMultiplier);

	// 충돌 컴포넌트 반환 함수
	UBoxComponent* GetCollisionComponent() const;
};
