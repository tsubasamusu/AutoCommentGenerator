// Copyright (c) 2025, tsubasamusu All rights reserved.

using UnrealBuildTool;

public class AutoCommentGenerator : ModuleRules
{
    public AutoCommentGenerator(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;

        bUseUnity = false;

        PublicIncludePaths.AddRange(new string[]
        {

        });

        PrivateIncludePaths.AddRange(new string[]
        {

        });

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "Engine"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "GraphEditor",
            "BlueprintGraph",
            "EditorStyle",
            "UnrealEd",
            "InputCore",
            "Projects",
            "Json",
            "JsonUtilities",
            "EngineSettings",
            "AssetRegistry"
        });

        DynamicallyLoadedModuleNames.AddRange(new string[]
        {

        });
    }
}