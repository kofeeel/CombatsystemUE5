// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CSGameMode.generated.h"

/**
 * 게임 모드 클래스
 */

class ACSCharacter;

// 웨이브 상태를 나타내는 열거형
UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,   // 시작 대기 중
	WaveInProgress,   // 웨이브 진행 중
	WaitingToComplete,// 완료 대기 중
	WaveComplete,     // 웨이브 완료
	GameOver          // 게임 오버
};


UCLASS()
class COMBATSYSTEM_API ACSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// ACSGameMode 생성자
	ACSGameMode();

	// 적 스폰 타이머 핸들
	FTimerHandle TimerHandle_EnemySpawner;

	// 다음 웨이브 시작 타이머 핸들
	FTimerHandle TimerHandle_NextWaveStart;

	// 스폰할 적의 수
	int32 NumberOfEnemiesToSpawn;

	// 현재 웨이브 수
	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	int32 WaveCount;

	// 웨이브 간의 시간 간격
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	// 새로운 적을 스폰하는 함수 (Blueprint에서 구현 가능)
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewEnemy();

	// 적 스폰 타이머가 만료되었을 때 호출되는 함수
	void SpawnEnemyTimerElapsed();

	// 웨이브 시작 함수
	void StartWave();
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnWaveStarted();

	// 웨이브 종료 함수
	void EndWave();

	// 다음 웨이브를 준비하는 함수
	void PrepareForNextWave();

	// 모든 적을 제거하는 함수
	void DestroyAllEnemies();

	// 게임 리셋 시간
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeToResetGame;

	// 게임 리셋 함수
	void ResetGame();

	// 게임 오버 처리 함수
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnGameOver();

	// 현재 웨이브 상태
	UPROPERTY(BlueprintReadOnly, Category = "GameMode")
	EWaveState WaveState;

	// 웨이브 상태 설정 함수
	void SetWaveState(EWaveState NewWaveState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	// 적 캐릭터 배열
	TArray<ACSCharacter*> Enemies;
	// 살아있는 적 캐릭터 배열
	TArray<ACSCharacter*> AliveEnemies;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void UpdateEnemiesCounter();

public:
	// 게임이 시작될 때 호출되는 함수
	virtual void StartPlay() override;

	// 매 프레임마다 호출되는 함수
	virtual void Tick(float DeltaSeconds) override;

	// 캐릭터가 죽었을 때 호출되는 함수
	void OnCharacterDead(ACSCharacter* DeadCharacter);

	// 적 추가 함수
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void AddEnemy(ACSCharacter* Enemy);

	// 살아있는 적의 수를 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetAliveEnemies();

	// 웨이브에 있는 적의 수를 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int32 GetWaveEnemies();
};
