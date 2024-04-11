// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ISourceControlModule.h"

class FToolBarBuilder;
class FMenuBuilder;

class FGitPushModule : public IModuleInterface
{
private:
	bool bShuttingDown = false;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void PluginButtonClicked();
	
private:

	void Startup();
	void RegisterMenus();
	void ProviderChanged(ISourceControlProvider& OldProvider, ISourceControlProvider& NewProvider);


private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
