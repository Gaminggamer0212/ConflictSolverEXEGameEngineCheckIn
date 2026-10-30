// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterStatDisplayStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCharacterStatDisplayStyle::StyleInstance = nullptr;

void FCharacterStatDisplayStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCharacterStatDisplayStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCharacterStatDisplayStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CharacterStatDisplayStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FCharacterStatDisplayStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CharacterStatDisplayStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CharacterStatDisplay")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CharacterStatDisplay.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FCharacterStatDisplayStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCharacterStatDisplayStyle::Get()
{
	return *StyleInstance;
}
