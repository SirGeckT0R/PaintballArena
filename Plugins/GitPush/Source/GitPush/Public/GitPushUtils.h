// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGitPushModule, Log, All);

namespace GitPushUtils {

	bool CheckGitAvailability(const FString& InPathToGitBinary);

	// Find the root of the Git repository, looking from the provided path and upward in its parent directories.
	bool FindRootDirectory(const FString& InPath, FString& OutRepositoryRoot);

	// Launch the Git command line process and extract its results & errors
	static bool RunCommand(const FString& InCommand, const FString& InPathToGitBinary, const FString& InRepositoryRoot, const TArray<FString>& InParameters, const TArray<FString>& InFiles, FString& OutResults, FString& OutErrors);

	FString FindGitBinaryPath();

	static FString RunPush();

	bool GetBranchName(const FString& InPathToGitBinary, const FString& InRepositoryRoot, FString& OutBranchName);
}