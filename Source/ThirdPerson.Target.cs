// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonTarget : TargetRules
{
	public ThirdPersonTarget(TargetInfo Target) : base(Target)
	{
        // 타겟 유형을 게임으로 설정
        Type = TargetType.Game;
        // 기본 빌드 설정 버전 2로 설정
        DefaultBuildSettings = BuildSettingsVersion.V2;
        // 이 타겟에 추가할 모듈 이름을 리스트에 추가
        ExtraModuleNames.Add("ThirdPerson");
	}
}
