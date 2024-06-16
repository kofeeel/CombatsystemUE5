// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

UCLASS(config = Game)
class AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** 캐릭터 뒤에 카메라를 배치하는 카메라 붐 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** 팔로우 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// AThirdPersonCharacter 생성자
	AThirdPersonCharacter();

	/** 기본 회전 속도 (deg/sec). 다른 스케일링이 최종 회전 속도에 영향을 줄 수 있음 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** 기본 위/아래 보기 속도 (deg/sec). 다른 스케일링이 최종 속도에 영향을 줄 수 있음 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:

	/** VR에서 HMD 방향을 재설정 */
	void OnResetVR();

	/** 전진/후진 입력 처리 함수 */
	void MoveForward(float Value);

	/** 좌우 입력 처리 함수 */
	void MoveRight(float Value);

	/**
	 * 주어진 비율로 회전하는 입력 처리 함수
	 * @param Rate	이 값은 정규화된 비율로, 1.0은 원하는 회전 속도의 100%를 의미
	 */
	void TurnAtRate(float Rate);

	/**
	 * 주어진 비율로 위/아래 보는 입력 처리 함수
	 * @param Rate	이 값은 정규화된 비율로, 1.0은 원하는 회전 속도의 100%를 의미
	 */
	void LookUpAtRate(float Rate);

	/** 터치 입력이 시작될 때의 처리기 */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** 터치 입력이 종료될 때의 처리기 */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn 인터페이스
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// APawn 인터페이스 끝

public:
	/** CameraBoom 서브오브젝트 반환 **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** FollowCamera 서브오브젝트 반환 **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
