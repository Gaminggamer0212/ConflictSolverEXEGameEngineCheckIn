#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "PerformanceTrackingSubsystem.generated.h"

UCLASS()
class PERFORMANCETRACKING_API UPerformanceTrackingSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Subsystem Lifecycle
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return bIsTracking; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UPerformanceTrackingSubsystem, STATGROUP_Tickables); }

	// Controls
	void StartTracking();
	void StopTracking();

	// Metrics Query
	float GetCurrentFPS() const { return CurrentFPS; }
	float GetUsedPhysicalMemoryMB() const { return UsedMemoryMB; }
	float GetUsedVram()const { return UsedVRAMMB; }

private:
	bool bIsTracking = false;
	float CurrentFPS = 0.0f;
	float UsedMemoryMB = 0.0f;
	float UsedVRAMMB = 0.0f;
};
