using UnrealBuildTool;

public class LovelyJsonDeveloper : ModuleRules
{
    public LovelyJsonDeveloper(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "JsonUtilities",
                "LovelyJson",
                "BlueprintGraph"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Json",
                "UnrealEd",
                "KismetCompiler",
                "ToolMenus",
                "Projects"
            }
        );
    }
}