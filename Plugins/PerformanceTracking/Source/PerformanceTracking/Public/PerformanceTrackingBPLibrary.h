#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PerformanceTrackingBPLibrary.generated.h"

UCLASS()
class PERFORMANCETRACKING_API UPerformanceTrackingBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Performance Tracking", meta = (WorldContext = "WorldContextObject"))
	static float GetCurrentFPS(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Performance Tracking", meta = (WorldContext = "WorldContextObject"))
	static float GetUsedMemoryMB(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Performance Tracking", meta = (WorldContext = "WorldContextObject"))
	static float GetUsedVram(const UObject* WorldContextObject);
};