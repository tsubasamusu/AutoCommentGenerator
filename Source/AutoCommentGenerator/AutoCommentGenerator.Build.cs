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
            "HTTP",
            "Json",
            "JsonUtilities",
            "UnrealEd",
            "GraphEditor"
        });

        DynamicallyLoadedModuleNames.AddRange(new string[]
        {

        });
    }
}