// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FNEOMatModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
    TArray<TSharedPtr<FString>> EffectOptions;
    TSharedPtr<FString> SelectedEffect;
    TSharedPtr<SWindow> EffectWindow;

    FLinearColor SelectColor = FLinearColor(1.0f, 0.0f, 0.67f, 1.0f);
    float SelectStrength = 1.67f;

    void InitializeEffects();
    void OpenEffectWin();
    void RegisterMenus();
    void GenerateMaterial(const FString& EffectName);
};
