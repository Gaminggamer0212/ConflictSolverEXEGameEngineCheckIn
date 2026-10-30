// Copyright (c) 2026 Krileon. All Rights Reserved.

#include "UINavigationEnhancedBlueprintLibrary.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/NavigationConfig.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"

void UUINavigationEnhancedBlueprintLibrary::ApplyUINavigationConfig(UUINavigationEnhancedDataAsset* DataAsset)
{
	if (!IsInGameThread())
	{
		return;
	}

	if (!DataAsset)
	{
		return;
	}

	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	TSharedRef<FNavigationConfig> NewConfig = DataAsset->BuildUINavigationConfig();
	FSlateApplication::Get().SetNavigationConfig(NewConfig);
}

void UUINavigationEnhancedBlueprintLibrary::ResetUINavigationConfig()
{
	if (!IsInGameThread())
	{
		return;
	}

	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	TSharedRef<FNavigationConfig> DefaultConfig = MakeShared<FNavigationConfig>();
	FSlateApplication::Get().SetNavigationConfig(DefaultConfig);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationEvent(const FKey& Key, const FModifierKeysState& Modifiers)
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	if (!IsInGameThread())
	{
		return;
	}

	if (!Key.IsValid())
	{
		return;
	}

	FSlateApplication& App = FSlateApplication::Get();

	FKeyEvent KeyDownEvent(Key, Modifiers, 0, false, 0, 0);

	App.ProcessKeyDownEvent(KeyDownEvent);

	FKeyEvent KeyUpEvent(Key, Modifiers, 0, false, 0, 0);

	App.ProcessKeyUpEvent(KeyUpEvent);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigation(EUINavigation Direction)
{
	FKey NavKey;
	FModifierKeysState Modifiers;

	switch (Direction)
	{
	case EUINavigation::Left:
		NavKey = EKeys::Left;
		break;
	case EUINavigation::Right:
		NavKey = EKeys::Right;
		break;
	case EUINavigation::Up:
		NavKey = EKeys::Up;
		break;
	case EUINavigation::Down:
		NavKey = EKeys::Down;
		break;
	case EUINavigation::Next:
		NavKey = EKeys::Tab;
		break;
	case EUINavigation::Previous:
		NavKey = EKeys::Tab;
		Modifiers = FModifierKeysState(true, false, false, false, false, false, false, false, false);
		break;
	default:
		return;
	}

	SimulateUINavigationEvent(NavKey, Modifiers);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationAction(EUINavigationAction Action)
{
	FKey NavKey;

	switch (Action)
	{
	case EUINavigationAction::Accept:
		NavKey = EKeys::Enter;
		break;
	case EUINavigationAction::Back:
		NavKey = EKeys::Escape;
		break;
	default:
		return;
	}

	SimulateUINavigationEvent(NavKey, FModifierKeysState());
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationKey(FKey Key)
{
	SimulateUINavigationEvent(Key, FModifierKeysState());
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationLeft()
{
	SimulateUINavigation(EUINavigation::Left);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationRight()
{
	SimulateUINavigation(EUINavigation::Right);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationUp()
{
	SimulateUINavigation(EUINavigation::Up);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationDown()
{
	SimulateUINavigation(EUINavigation::Down);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationNext()
{
	SimulateUINavigation(EUINavigation::Next);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationPrevious()
{
	SimulateUINavigation(EUINavigation::Previous);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationAccept()
{
	SimulateUINavigationAction(EUINavigationAction::Accept);
}

void UUINavigationEnhancedBlueprintLibrary::SimulateUINavigationBack()
{
	SimulateUINavigationAction(EUINavigationAction::Back);
}
