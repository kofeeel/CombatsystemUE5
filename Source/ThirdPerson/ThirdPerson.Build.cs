// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThirdPerson : ModuleRules
{
	public ThirdPerson(ReadOnlyTargetRules Target) : base(Target)
	{
        // PCH 사용 모드를 명시적으로 또는 공유 PCH로 설정
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        // 공개 의존성 모듈 이름을 추가
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
