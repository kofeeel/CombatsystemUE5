// ThirdPersonCharacter.cpp: 캐릭터 클래스 소스 파일

#include "ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

// AThirdPersonCharacter 생성자
AThirdPersonCharacter::AThirdPersonCharacter()
{
    // 충돌 캡슐 크기 설정
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // 입력을 위한 회전 속도 설정
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // 컨트롤러가 회전할 때 캐릭터가 회전하지 않도록 설정
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // 캐릭터 이동 설정
    GetCharacterMovement()->bOrientRotationToMovement = true; // 입력 방향으로 캐릭터가 이동하도록 설정
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 이 회전 속도로 회전
    GetCharacterMovement()->JumpZVelocity = 600.f; // 점프 높이
    GetCharacterMovement()->AirControl = 0.2f; // 공중 제어력

    // 카메라 붐 생성 (충돌 시 플레이어 쪽으로 당김)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // 캐릭터 뒤의 이 거리에서 카메라가 따라옴
    CameraBoom->bUsePawnControlRotation = true; // 컨트롤러에 따라 붐이 회전하도록 설정

    // 팔로우 카메라 생성
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 붐 끝에 카메라를 부착하고 붐이 컨트롤러 방향에 맞게 조정하도록 함
    FollowCamera->bUsePawnControlRotation = false; // 카메라가 붐에 대해 회전하지 않도록 설정

    // 참고: 메쉬 컴포넌트(캐릭터에서 상속된)의 스켈레탈 메쉬 및 애님 블루프린트 참조는
    // MyCharacter라는 파생 블루프린트 자산에서 설정됨 (C++에서 직접 콘텐츠 참조를 피하기 위함)
}

//////////////////////////////////////////////////////////////////////////
// 입력

// 플레이어 입력 설정 함수
void AThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // 게임플레이 키 바인딩 설정
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCharacter::MoveRight);

    // 다른 종류의 장치를 다르게 처리하기 위해 두 가지 버전의 회전 바인딩이 있음
    // "turn"은 마우스와 같이 절대 델타를 제공하는 장치를 처리
    // "turnrate"는 아날로그 조이스틱처럼 변경 비율로 처리하려는 장치를 위해 사용
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonCharacter::LookUpAtRate);

    // 터치 장치 처리
    PlayerInputComponent->BindTouch(IE_Pressed, this, &AThirdPersonCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AThirdPersonCharacter::TouchStopped);

    // VR 헤드셋 기능
    PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AThirdPersonCharacter::OnResetVR);
}

// VR 초기화 함수
void AThirdPersonCharacter::OnResetVR()
{
    // Unreal Editor에서 '기능 추가'를 통해 프로젝트에 ThirdPerson을 추가하면 ThirdPerson.Build.cs에 HeadMountedDisplay에 대한 의존성이 자동으로 전파되지 않음
    // 그 결과 링커 오류가 발생할 수 있음
    // 다음 중 하나를 수행해야 함:
    // 1. 빌드가 성공하도록 [YourProject].Build.cs의 PublicDependencyModuleNames에 "HeadMountedDisplay"를 추가 (VR 지원 시 적절함)
    // 2. 아래의 ResetOrientationAndPosition 호출을 주석 처리하거나 삭제 (VR 지원하지 않는 경우 적절함)
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

// 터치 시작 시 호출되는 함수
void AThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

// 터치 종료 시 호출되는 함수
void AThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}

// 회전 속도 함수
void AThirdPersonCharacter::TurnAtRate(float Rate)
{
    // 회전 비율 정보를 기반으로 이 프레임의 델타 계산
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// 위아래 보기 속도 함수
void AThirdPersonCharacter::LookUpAtRate(float Rate)
{
    // 회전 비율 정보를 기반으로 이 프레임의 델타 계산
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// 전진 및 후진 이동 함수
void AThirdPersonCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // 어느 방향이 앞으로 가는지 확인
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 앞으로 가는 벡터 가져오기
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

// 좌우 이동 함수
void AThirdPersonCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // 어느 방향이 오른쪽인지 확인
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 오른쪽 벡터 가져오기
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // 해당 방향으로 이동 추가
        AddMovementInput(Direction, Value);
    }
}
