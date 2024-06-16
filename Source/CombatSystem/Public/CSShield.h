// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSShield.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class COMBATSYSTEM_API ACSShield : public AActor
{
	GENERATED_BODY()

public:
	// 이 액터의 기본 값을 설정합니다.
	ACSShield();

protected:
	// 게임이 시작되거나 스폰될 때 호출됩니다.
	virtual void BeginPlay() override;

	// 컴포넌트 카테고리에서 메쉬 컴포넌트를 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

public:
	// 매 프레임마다 호출됩니다.
	virtual void Tick(float DeltaTime) override;

};
