// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatSystem : ModuleRules
{
	public CombatSystem(ReadOnlyTargetRules Target) : base(Target)
	{
        // PCH 사용 모드를 명시적 또는 공유 PCH로 설정
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // 공개 의존성 모듈 이름을 추가
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "PhysicsCore" });

        // 비공개 의존성 모듈 이름을 추가
        PrivateDependencyModuleNames.AddRange(new string[] {});

        // Slate UI를 사용하는 경우 주석 해제
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // 온라인 기능을 사용하는 경우 주석 해제
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // OnlineSubsystemSteam을 포함하려면, uproject 파일의 plugins 섹션에 Enabled 속성을 true로 설정하여 추가
    }
}
