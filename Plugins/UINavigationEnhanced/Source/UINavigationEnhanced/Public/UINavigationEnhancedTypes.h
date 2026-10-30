// Copyright (c) 2026 Krileon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Types/SlateEnums.h"

#include "UINavigationEnhancedTypes.generated.h"

USTRUCT(BlueprintType)
struct UINAVIGATIONENHANCED_API FUINavigationEnhancedKeyMapping
{
	GENERATED_BODY()

	FUINavigationEnhancedKeyMapping() = default;

	FUINavigationEnhancedKeyMapping(const FKey& InKey, EUINavigation InDirection)
		: NavigationKey(InKey)
		, NavigationDirection(InDirection)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation")
	FKey NavigationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation")
	EUINavigation NavigationDirection = EUINavigation::Invalid;
};

USTRUCT(BlueprintType)
struct UINAVIGATIONENHANCED_API FUINavigationEnhancedActionMapping
{
	GENERATED_BODY()

	FUINavigationEnhancedActionMapping() = default;

	FUINavigationEnhancedActionMapping(const FKey& InKey, EUINavigationAction InAction)
		: NavigationKey(InKey)
		, NavigationAction(InAction)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation")
	FKey NavigationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation")
	EUINavigationAction NavigationAction = EUINavigationAction::Invalid;
};