// Copyright Epic Games, Inc. All Rights Reserved.

#include "GitPush.h"
#include "GitPushStyle.h"
#include "GitPushUtils.h"
#include "GitPushCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Modules/ModuleManager.h"
#include <SourceControlHelpers.h>

static const FName GitPushTabName("GitPush");

#define LOCTEXT_NAMESPACE "FGitPushModule"

void FGitPushModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGitPushStyle::Initialize();
	FGitPushStyle::ReloadTextures();

	FGitPushCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGitPushCommands::Get().GitPushPushToRemote,
		FExecuteAction::CreateRaw(this, &FGitPushModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGitPushModule::Startup));
}

void FGitPushModule::ShutdownModule()
{
	this->bShuttingDown = true;
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGitPushStyle::Shutdown();

	FGitPushCommands::Unregister();
}

void FGitPushModule::PluginButtonClicked()
{
	FString errors = GitPushUtils::RunPush();
	FText DialogText = LOCTEXT("GitPushPushToRemoteText", "Successfully pushed new changes to remote repository!");
	if (errors != "None") {
		DialogText = FText::FromString(errors);
	}
	
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FGitPushModule::Startup()
{
	ISourceControlModule& SourceControl = FModuleManager::LoadModuleChecked<ISourceControlModule>("GitSourceControl");
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	SourceControl.Get().RegisterProviderChanged(FSourceControlProviderChanged::FDelegate::CreateRaw(this, &FGitPushModule::ProviderChanged));

	if (USourceControlHelpers::CurrentProvider() == "Git") {
		RegisterMenus();
	}
}

void FGitPushModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("StatusBar.ToolBar.SourceControl");		
	if (ToolbarMenu == nullptr) {
		return;
	}
	FToolMenuSection* Section = ToolbarMenu->FindSection("SourceControlActions");
	if (Section == nullptr) {
		return;
	}
	Section->AddMenuEntryWithCommandList(FGitPushCommands::Get().GitPushPushToRemote, PluginCommands);
}

void FGitPushModule::ProviderChanged(ISourceControlProvider& OldProvider, ISourceControlProvider& NewProvider)
{
	if (bShuttingDown) {
		return;
	}

	FString providerName = USourceControlHelpers::CurrentProvider();
	if(providerName == "Git") {
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("StatusBar.ToolBar.SourceControl");
		if (ToolbarMenu == nullptr) {
			return;
		}
		FToolMenuSection* Section = ToolbarMenu->FindSection("SourceControlActions");
		if (Section == nullptr) {
			return;
		}
		Section->AddMenuEntryWithCommandList(FGitPushCommands::Get().GitPushPushToRemote, PluginCommands);
	}
	else {
		UToolMenus::Get()->RemoveEntry("StatusBar.ToolBar.SourceControl", "SourceControlActions", TEXT("GitPushPushToRemote"));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGitPushModule, GitPush)