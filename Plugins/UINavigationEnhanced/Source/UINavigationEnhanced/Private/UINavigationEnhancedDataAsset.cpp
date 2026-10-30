// Copyright (c) 2026 Krileon. All Rights Reserved.

#include "UINavigationEnhancedDataAsset.h"

#include "Framework/Application/NavigationConfig.h"
#include "InputCoreTypes.h"

class FDataDrivenNavigationConfig : public FNavigationConfig
{
public:
	TArray<FKey> CustomAnalogHorizontalKeys;
	TArray<FKey> CustomAnalogVerticalKeys;

	virtual bool IsAnalogHorizontalKey(const FKey& InKey) const override
	{
		if (CustomAnalogHorizontalKeys.Num() > 0)
		{
			return CustomAnalogHorizontalKeys.Contains(InKey);
		}
		return FNavigationConfig::IsAnalogHorizontalKey(InKey);
	}

	virtual bool IsAnalogVerticalKey(const FKey& InKey) const override
	{
		if (CustomAnalogVerticalKeys.Num() > 0)
		{
			return CustomAnalogVerticalKeys.Contains(InKey);
		}
		return FNavigationConfig::IsAnalogVerticalKey(InKey);
	}
};

UUINavigationEnhancedDataAsset::UUINavigationEnhancedDataAsset()
{
	ResetUINavigationConfig();
}

void UUINavigationEnhancedDataAsset::ResetUINavigationConfig()
{
	bTabNavigation    = true;
	bKeyNavigation    = true;
	bAnalogNavigation = true;

	AnalogNavigationHorizontalThreshold = 0.25f;
	AnalogNavigationVerticalThreshold   = 0.25f;

	AnalogHorizontalKeys.Reset();
	AnalogVerticalKeys.Reset();

	ActionMappings.Reset();
	bUseDefaultActionMappings = true;

	KeyMappings.Reset();
	bUseDefaultKeyMappings = true;
}

TSharedRef<FNavigationConfig> UUINavigationEnhancedDataAsset::BuildUINavigationConfig() const
{
	TSharedRef<FDataDrivenNavigationConfig> Config = MakeShared<FDataDrivenNavigationConfig>();

	Config->bTabNavigation = bTabNavigation;
	Config->bKeyNavigation = bKeyNavigation;
	Config->bAnalogNavigation = bAnalogNavigation;

	Config->AnalogNavigationHorizontalThreshold = AnalogNavigationHorizontalThreshold;
	Config->AnalogNavigationVerticalThreshold = AnalogNavigationVerticalThreshold;

	Config->CustomAnalogHorizontalKeys = AnalogHorizontalKeys;
	Config->CustomAnalogVerticalKeys = AnalogVerticalKeys;

	if (!bUseDefaultActionMappings)
	{
		Config->KeyActionRules.Reset();
	}

	for (const FUINavigationEnhancedActionMapping& Mapping : ActionMappings)
	{
		if (Mapping.NavigationKey.IsValid() &&
			Mapping.NavigationAction != EUINavigationAction::Invalid)
		{
			Config->KeyActionRules.Emplace(Mapping.NavigationKey, Mapping.NavigationAction);
		}
	}

	if (!bUseDefaultKeyMappings)
	{
		Config->KeyEventRules.Reset();
	}

	for (const FUINavigationEnhancedKeyMapping& Mapping : KeyMappings)
	{
		if (Mapping.NavigationKey.IsValid() &&
			Mapping.NavigationDirection != EUINavigation::Invalid)
		{
			Config->KeyEventRules.Emplace(Mapping.NavigationKey, Mapping.NavigationDirection);
		}
	}

	return Config;
}
