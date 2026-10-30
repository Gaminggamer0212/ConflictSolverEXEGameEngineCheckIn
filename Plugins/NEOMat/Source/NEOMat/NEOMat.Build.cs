// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NEOMat : ModuleRules
{
	public NEOMat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// this section anyone can read these
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
            }
			);
				
		// this section is for the privates stuff, so only the module will read these headrers
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
            }
			);
			
		// dependencies gives acess to unreal's editor, menu system, content drawer, asset creation APIs
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Engine",
                "CoreUObject"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				
				"InputCore", // key stuff
				"SlateCore", // slatecore is base for ui
				"Slate", // different than slate core, the actual ui stuff

                "UnrealEd", //this is unreal's editor module, use to acess material editor
				"ToolMenus", // tool menus, addign buttons to the editor
				"LevelEditor", // level editor, this is main editor window
				"ContentBrowser", // content browser, to acess the assets contents
				"AssetTools", // asset tools, create new asets api
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
