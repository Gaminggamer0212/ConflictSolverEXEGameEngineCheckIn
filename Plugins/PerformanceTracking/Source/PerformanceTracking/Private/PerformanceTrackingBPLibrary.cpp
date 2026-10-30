#include "PerformanceTrackingBPLibrary.h"
#include "PerformanceTrackingSubsystem.h"
#include "Engine/GameInstance.h"

float UPerformanceTrackingBPLibrary::GetCurrentFPS(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return 0.0f;

	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UPerformanceTrackingSubsystem* Tracker = GI->GetSubsystem<UPerformanceTrackingSubsystem>())
			{
				return Tracker->GetCurrentFPS();
			}
		}
	}
	return 0.0f;
}

float UPerformanceTrackingBPLibrary::GetUsedMemoryMB(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return 0.0f;

	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UPerformanceTrackingSubsystem* Tracker = GI->GetSubsystem<UPerformanceTrackingSubsystem>())
			{
				return Tracker->GetUsedPhysicalMemoryMB();
			}
		}
	}
	return 0.0f;
}

float UPerformanceTrackingBPLibrary::GetUsedVram(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return 0.0f;
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UPerformanceTrackingSubsystem* Tracker = GI->GetSubsystem<UPerformanceTrackingSubsystem>())
			{
				return Tracker->GetUsedVram();
			}
		}
	}


	return 0.0f;
}