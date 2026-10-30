using UnrealBuildTool;

public class PerformanceTracking : ModuleRules
{
    public PerformanceTracking(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "RHI",
                "RenderCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "DeveloperSettings"
            }
        );
    }
}