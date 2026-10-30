// Copyright (c) 2026 Krileon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Framework/Application/NavigationConfig.h" 
#include "UINavigationEnhancedTypes.h"

#include "UINavigationEnhancedDataAsset.generated.h"

UCLASS(BlueprintType, meta = (DisplayName = "UI Navigation Config"))
class UINAVIGATIONENHANCED_API UUINavigationEnhancedDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UUINavigationEnhancedDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|General", meta = (DisplayName = "Tab Navigation"))
	bool bTabNavigation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|General", meta = (DisplayName = "Key Navigation"))
	bool bKeyNavigation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|General", meta = (DisplayName = "Analog Navigation"))
	bool bAnalogNavigation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Analog", meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Analog Horizontal Threshold"))
	float AnalogNavigationHorizontalThreshold = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Analog", meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Analog Vertical Threshold"))
	float AnalogNavigationVerticalThreshold = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Analog", meta = (DisplayName = "Analog Horizontal Keys"))
	TArray<FKey> AnalogHorizontalKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Analog", meta = (DisplayName = "Analog Vertical Keys"))
	TArray<FKey> AnalogVerticalKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Actions", meta = (DisplayName = "Action Mappings"))
	TArray<FUINavigationEnhancedActionMapping> ActionMappings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Actions", meta = (DisplayName = "Use Default Action Mappings"))
	bool bUseDefaultActionMappings = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Keys", meta = (DisplayName = "Key Mappings"))
	TArray<FUINavigationEnhancedKeyMapping> KeyMappings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Navigation|Keys", meta = (DisplayName = "Use Default Key Mappings"))
	bool bUseDefaultKeyMappings = true;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "UI Navigation", meta = (DisplayName = "Reset"))
	void ResetUINavigationConfig();

	TSharedRef<FNavigationConfig> BuildUINavigationConfig() const;
};
