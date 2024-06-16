// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonGameMode.h"
#include "ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

// AThirdPersonGameMode 생성자
AThirdPersonGameMode::AThirdPersonGameMode()
{
	// 기본 폰 클래스를 블루프린트된 캐릭터로 설정
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
