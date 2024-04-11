// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GitPushStyle.h"

class FGitPushCommands : public TCommands<FGitPushCommands>
{
public:

	FGitPushCommands()
		: TCommands<FGitPushCommands>(TEXT("GitPushPushToRemote"), NSLOCTEXT("Contexts", "GitPushPushToRemote", "GitPush Plugin"), NAME_None, FGitPushStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > GitPushPushToRemote;
};
