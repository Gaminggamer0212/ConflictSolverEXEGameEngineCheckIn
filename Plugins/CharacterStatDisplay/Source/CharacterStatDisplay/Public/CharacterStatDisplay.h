// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Containers/Ticker.h"
#include "Widgets/Views/SListView.h"

class FToolBarBuilder;
class FMenuBuilder;

struct FStatLine
{
	FString Label;
	double Min = 0, MinMod = 0;
	double Max = 0, MaxMod = 0;
	double Cur = 0, CurMod = 0;
};

struct FCombatantRow
{
	FString ActorName;
	TArray<FStatLine> Lines;
};

class FCharacterStatDisplayModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	void RefreshRows();
	bool OnTick(float DeltaTime);
	UWorld* GetTargetWorld() const;
	UClass* GetCompClass();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TArray<TSharedPtr<FCombatantRow>> Rows;
	TSharedPtr<SListView<TSharedPtr<FCombatantRow>>> ListView;
	TWeakObjectPtr<UClass> CachedCompClass;
	FTSTicker::FDelegateHandle TickHandle;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
