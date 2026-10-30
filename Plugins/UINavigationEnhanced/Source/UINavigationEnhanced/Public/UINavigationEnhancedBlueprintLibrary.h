// Copyright (c) 2026 Krileon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/SlateEnums.h"
#include "InputCoreTypes.h"
#include "UINavigationEnhancedDataAsset.h"

#include "UINavigationEnhancedBlueprintLibrary.generated.h"

UCLASS(meta = (DisplayName = "UI Navigation Enhanced Blueprint Library"))
class UINAVIGATIONENHANCED_API UUINavigationEnhancedBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI Navigation", meta = (DisplayName = "Apply UI Navigation Config"))
	static void ApplyUINavigationConfig(UUINavigationEnhancedDataAsset* DataAsset);

	UFUNCTION(BlueprintCallable, Category = "UI Navigation", meta = (DisplayName = "Reset UI Navigation Config"))
	static void ResetUINavigationConfig();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction"))
	static void SimulateUINavigation(EUINavigation Direction);

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Action"))
	static void SimulateUINavigationAction(EUINavigationAction Action);

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Key"))
	static void SimulateUINavigationKey(FKey Key);

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction Left"))
	static void SimulateUINavigationLeft();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction Right"))
	static void SimulateUINavigationRight();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction Up"))
	static void SimulateUINavigationUp();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction Down"))
	static void SimulateUINavigationDown();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction Next"))
	static void SimulateUINavigationNext();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Direction Previous"))
	static void SimulateUINavigationPrevious();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Action Accept"))
	static void SimulateUINavigationAccept();

	UFUNCTION(BlueprintCallable, Category = "UI Navigation|Simulate", meta = (DisplayName = "Simulate UI Action Back"))
	static void SimulateUINavigationBack();

private:

	static void SimulateUINavigationEvent(const FKey& Key, const FModifierKeysState& Modifiers);
};
