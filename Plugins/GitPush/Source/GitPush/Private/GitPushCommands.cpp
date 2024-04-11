// Copyright Epic Games, Inc. All Rights Reserved.

#include "GitPushCommands.h"

#define LOCTEXT_NAMESPACE "FGitPushModule"

void FGitPushCommands::RegisterCommands()
{
	UI_COMMAND(GitPushPushToRemote, "Push changes", "Push changes to remote repository", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
