// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterStatDisplayCommands.h"

#define LOCTEXT_NAMESPACE "FCharacterStatDisplayModule"

void FCharacterStatDisplayCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "CharacterStatDisplay", "Bring up CharacterStatDisplay window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
