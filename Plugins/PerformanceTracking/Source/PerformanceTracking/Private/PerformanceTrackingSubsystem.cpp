#include "PerformanceTrackingSubsystem.h"
#include "HAL/PlatformMemory.h"
#include "RHI.h"
void UPerformanceTrackingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	StartTracking();
}

void UPerformanceTrackingSubsystem::Deinitialize()
{
	StopTracking();
	Super::Deinitialize();
}

void UPerformanceTrackingSubsystem::StartTracking()
{
	bIsTracking = true;
}

void UPerformanceTrackingSubsystem::StopTracking()
{
	bIsTracking = false;
}

void UPerformanceTrackingSubsystem::Tick(float DeltaTime)
{
	if (!bIsTracking) return;

	// Calculate Frame Rate
	if (DeltaTime > 0.0f)
	{
		CurrentFPS = 1.0f / DeltaTime;
	}

	// Calculate Memory Metrics
	FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
	UsedMemoryMB = static_cast<float>(MemStats.UsedPhysical) / (1024.0f * 1024.0f);

	//Calculate Vram Usage
	FTextureMemoryStats TextureMemStats;
	RHIGetTextureMemoryStats(TextureMemStats);

	// StreamingMemorySize holds bytes allocated to streaming textures in the pool
	if (TextureMemStats.StreamingMemorySize > 0)
	{
		UsedVRAMMB = static_cast<float>(TextureMemStats.StreamingMemorySize) / (1024.0f * 1024.0f);
	}
	else if (TextureMemStats.DedicatedVideoMemory > 0)
	{
		UsedVRAMMB = static_cast<float>(TextureMemStats.DedicatedVideoMemory) / (1024.0f * 1024.0f);
	}
	else
	{
		UsedVRAMMB = 0.0f;
	}
}