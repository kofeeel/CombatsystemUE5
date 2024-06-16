// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UDamageType;
class UParticleSystem;
class UNiagaraSystem;
class ACSCharacter;

UCLASS()
class COMBATSYSTEM_API ACSWeapon : public AActor
{
	GENERATED_BODY()

public:
	// 이 액터의 기본 값을 설정합니다.
	ACSWeapon();

protected:
	// 게임이 시작되거나 스폰될 때 호출됩니다.
	virtual void BeginPlay() override;

	// 컴포넌트 카테고리에서 메쉬 컴포넌트를 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	// 물리적 표면 유형 및 충돌 지점을 기반으로 충격 효과를 재생합니다.
	virtual void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	// 무기 카테고리에서 피해량을 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float DamageAmount;

	// 무기 카테고리에서 피해 유형을 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	// 캐릭터 참조를 저장
	ACSCharacter* Character;

public:
	// 새로운 캐릭터를 설정합니다.
	void SetCharacter(ACSCharacter* NewCharacter);

	// 매 프레임마다 호출됩니다.
	virtual void Tick(float DeltaTime) override;
};
