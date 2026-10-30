// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "CharacterStatDisplayStyle.h"

class FCharacterStatDisplayCommands : public TCommands<FCharacterStatDisplayCommands>
{
public:

	FCharacterStatDisplayCommands()
		: TCommands<FCharacterStatDisplayCommands>(TEXT("CharacterStatDisplay"), NSLOCTEXT("Contexts", "CharacterStatDisplay", "CharacterStatDisplay Plugin"), NAME_None, FCharacterStatDisplayStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};