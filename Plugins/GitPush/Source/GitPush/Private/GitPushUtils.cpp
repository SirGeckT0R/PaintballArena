// Copyright Epic Games, Inc. All Rights Reserved.

#include "GitPushUtils.h"

DEFINE_LOG_CATEGORY(LogGitPushModule);

bool GitPushUtils::CheckGitAvailability(const FString& InPathToGitBinary)
{
	FString InfoMessages;
	FString ErrorMessages;
	bool bGitAvailable = RunCommand(TEXT("version"), InPathToGitBinary, FString(), TArray<FString>(), TArray<FString>(), InfoMessages, ErrorMessages);
	if (bGitAvailable)
	{
		if (!InfoMessages.Contains("git"))
		{
			bGitAvailable = false;
		}
	}

	return bGitAvailable;
}

// Find the root of the Git repository, looking from the provided path and upward in its parent directories.
bool GitPushUtils::FindRootDirectory(const FString& InPath, FString& OutRepositoryRoot)
{
	bool bFound = false;
	FString PathToGitSubdirectory;
	OutRepositoryRoot = InPath;

	auto TrimTrailing = [](FString& Str, const TCHAR Char)
		{
			int32 Len = Str.Len();
			while (Len && Str[Len - 1] == Char)
			{
				Str.LeftChopInline(1);
				Len = Str.Len();
			}
		};

	TrimTrailing(OutRepositoryRoot, '\\');
	TrimTrailing(OutRepositoryRoot, '/');

	while (!bFound && !OutRepositoryRoot.IsEmpty())
	{
		// Look for the ".git" subdirectory (or file) present at the root of every Git repository
		PathToGitSubdirectory = OutRepositoryRoot / TEXT(".git");
		bFound = IFileManager::Get().DirectoryExists(*PathToGitSubdirectory) || IFileManager::Get().FileExists(*PathToGitSubdirectory);
		if (!bFound)
		{
			int32 LastSlashIndex;
			if (OutRepositoryRoot.FindLastChar('/', LastSlashIndex))
			{
				OutRepositoryRoot.LeftInline(LastSlashIndex);
			}
			else
			{
				OutRepositoryRoot.Empty();
			}
		}
	}
	if (!bFound)
	{
		OutRepositoryRoot = InPath; // If not found, return the provided dir as best possible root.
	}
	return bFound;
}

// Launch the Git command line process and extract its results & errors
bool GitPushUtils::RunCommand(const FString& InCommand, const FString& InPathToGitBinary, const FString& InRepositoryRoot, const TArray<FString>& InParameters, const TArray<FString>& InFiles, FString& OutResults, FString& OutErrors)
{
	int32 ReturnCode = 0;
	FString FullCommand;
	FString LogableCommand; // short version of the command for logging purpose

	if (!InRepositoryRoot.IsEmpty())
	{
		FString RepositoryRoot = InRepositoryRoot;

		// Detect a "migrate asset" scenario (a "git add" command is applied to files outside the current project)
		if ((InFiles.Num() > 0) && !FPaths::IsRelative(InFiles[0]) && !InFiles[0].StartsWith(InRepositoryRoot))
		{
			// in this case, find the git repository (if any) of the destination Project
			FString DestinationRepositoryRoot;
			if (FindRootDirectory(FPaths::GetPath(InFiles[0]), DestinationRepositoryRoot))
			{
				RepositoryRoot = DestinationRepositoryRoot; // if found use it for the "add" command (else not, to avoid producing one more error in logs)
			}
		}

		// Specify the working copy (the root) of the git repository (before the command itself)
		FullCommand = TEXT("-C \"");
		FullCommand += RepositoryRoot;
		FullCommand += TEXT("\" ");
	}
	// then the git command itself ("status", "log", "commit"...)
	LogableCommand += InCommand;

	// Append to the command all parameters, and then finally the files
	for (const auto& Parameter : InParameters)
	{
		LogableCommand += TEXT(" ");
		LogableCommand += Parameter;
	}
	for (const auto& File : InFiles)
	{
		LogableCommand += TEXT(" \"");
		LogableCommand += File;
		LogableCommand += TEXT("\"");
	}
	// Also, Git does not have a "--non-interactive" option, as it auto-detects when there are no connected standard input/output streams

	FullCommand += LogableCommand;

#if UE_BUILD_DEBUG
	UE_LOG(LogGitPushModule, Log, TEXT("RunCommand: 'git %s'"), *LogableCommand);
#endif

	FString PathToGitOrEnvBinary = InPathToGitBinary;
#if PLATFORM_MAC
	// The Cocoa application does not inherit shell environment variables, so add the path expected to have git-lfs to PATH
	FString PathEnv = FPlatformMisc::GetEnvironmentVariable(TEXT("PATH"));
	FString GitInstallPath = FPaths::GetPath(InPathToGitBinary);

	TArray<FString> PathArray;
	PathEnv.ParseIntoArray(PathArray, FPlatformMisc::GetPathVarDelimiter());
	bool bHasGitInstallPath = false;
	for (auto Path : PathArray)
	{
		if (GitInstallPath.Equals(Path, ESearchCase::CaseSensitive))
		{
			bHasGitInstallPath = true;
			break;
		}
	}

	if (!bHasGitInstallPath)
	{
		PathToGitOrEnvBinary = FString("/usr/bin/env");
		FullCommand = FString::Printf(TEXT("PATH=\"%s%s%s\" \"%s\" %s"), *GitInstallPath, FPlatformMisc::GetPathVarDelimiter(), *PathEnv, *InPathToGitBinary, *FullCommand);
	}
#endif
	FPlatformProcess::ExecProcess(*PathToGitOrEnvBinary, *FullCommand, &ReturnCode, &OutResults, &OutErrors);

#if UE_BUILD_DEBUG

	if (OutResults.IsEmpty())
	{
		UE_LOG(LogGitPushModule, Log, TEXT("RunCommand: 'OutResults=n/a'"));
	}
	else
	{
		UE_LOG(LogGitPushModule, Log, TEXT("RunCommand(%s): OutResults=\n%s"), *InCommand, *OutResults);
	}

	if (ReturnCode != 0)
	{
		if (OutErrors.IsEmpty())
		{
			UE_LOG(LogGitPushModule, Warning, TEXT("RunCommand: 'OutErrors=n/a'"));
		}
		else
		{
			UE_LOG(LogGitPushModule, Warning, TEXT("RunCommand(%s): OutErrors=\n%s"), *InCommand, *OutErrors);
		}
	}
#endif

	return ReturnCode == 0;
}

FString GitPushUtils::FindGitBinaryPath()
{
#if PLATFORM_WINDOWS
	// 1) First of all, look into standard install directories
	// NOTE using only "git" (or "git.exe") relying on the "PATH" envvar does not always work as expected, depending on the installation:
	// If the PATH is set with "git/cmd" instead of "git/bin",
	// "git.exe" launch "git/cmd/git.exe" that redirect to "git/bin/git.exe" and ExecProcess() is unable to catch its outputs streams.
	// First check the 64-bit program files directory:
	FString GitBinaryPath(TEXT("C:/Program Files/Git/bin/git.exe"));
	bool bFound = CheckGitAvailability(GitBinaryPath);
	if (!bFound)
	{
		// otherwise check the 32-bit program files directory.
		GitBinaryPath = TEXT("C:/Program Files (x86)/Git/bin/git.exe");
		bFound = CheckGitAvailability(GitBinaryPath);
	}
	if (!bFound)
	{
		// else the install dir for the current user: C:\Users\UserName\AppData\Local\Programs\Git\cmd
		FString AppDataLocalPath = FPlatformMisc::GetEnvironmentVariable(TEXT("LOCALAPPDATA"));
		GitBinaryPath = FString::Printf(TEXT("%s/Programs/Git/cmd/git.exe"), *AppDataLocalPath);
		bFound = CheckGitAvailability(GitBinaryPath);
	}

	// 2) Else, look for the version of Git bundled with SmartGit "Installer with JRE"
	if (!bFound)
	{
		GitBinaryPath = TEXT("C:/Program Files (x86)/SmartGit/git/bin/git.exe");
		bFound = CheckGitAvailability(GitBinaryPath);
		if (!bFound)
		{
			// If git is not found in "git/bin/" subdirectory, try the "bin/" path that was in use before
			GitBinaryPath = TEXT("C:/Program Files (x86)/SmartGit/bin/git.exe");
			bFound = CheckGitAvailability(GitBinaryPath);
		}
	}

	// 3) Else, look for the local_git provided by SourceTree
	if (!bFound)
	{
		// C:\Users\UserName\AppData\Local\Atlassian\SourceTree\git_local\bin
		FString AppDataLocalPath = FPlatformMisc::GetEnvironmentVariable(TEXT("LOCALAPPDATA"));
		GitBinaryPath = FString::Printf(TEXT("%s/Atlassian/SourceTree/git_local/bin/git.exe"), *AppDataLocalPath);
		bFound = CheckGitAvailability(GitBinaryPath);
	}

	// 4) Else, look for the PortableGit provided by GitHub Desktop
	if (!bFound)
	{
		// The latest GitHub Desktop adds its binaries into the local appdata directory:
		// C:\Users\UserName\AppData\Local\GitHub\PortableGit_c2ba306e536fdf878271f7fe636a147ff37326ad\cmd
		FString AppDataLocalPath = FPlatformMisc::GetEnvironmentVariable(TEXT("LOCALAPPDATA"));
		FString SearchPath = FString::Printf(TEXT("%s/GitHub/PortableGit_*"), *AppDataLocalPath);
		TArray<FString> PortableGitFolders;
		IFileManager::Get().FindFiles(PortableGitFolders, *SearchPath, false, true);
		if (PortableGitFolders.Num() > 0)
		{
			// FindFiles just returns directory names, so we need to prepend the root path to get the full path.
			GitBinaryPath = FString::Printf(TEXT("%s/GitHub/%s/cmd/git.exe"), *AppDataLocalPath, *(PortableGitFolders.Last())); // keep only the last PortableGit found
			bFound = CheckGitAvailability(GitBinaryPath);
			if (!bFound)
			{
				// If Portable git is not found in "cmd/" subdirectory, try the "bin/" path that was in use before
				GitBinaryPath = FString::Printf(TEXT("%s/GitHub/%s/bin/git.exe"), *AppDataLocalPath, *(PortableGitFolders.Last())); // keep only the last PortableGit found
				bFound = CheckGitAvailability(GitBinaryPath);
			}
		}
	}

	// 5) Else, look for the version of Git bundled with Tower
	if (!bFound)
	{
		GitBinaryPath = TEXT("C:/Program Files (x86)/fournova/Tower/vendor/Git/bin/git.exe");
		bFound = CheckGitAvailability(GitBinaryPath);
	}

#elif PLATFORM_MAC
	// 1) First of all, look for the version of git provided by official git
	FString GitBinaryPath = TEXT("/usr/local/git/bin/git");
	bool bFound = CheckGitAvailability(GitBinaryPath);

	// 2) Else, look for the version of git provided by Homebrew
	if (!bFound)
	{
		GitBinaryPath = TEXT("/usr/local/bin/git");
		bFound = CheckGitAvailability(GitBinaryPath);
	}

	// 3) Else, look for the version of git provided by MacPorts
	if (!bFound)
	{
		GitBinaryPath = TEXT("/opt/local/bin/git");
		bFound = CheckGitAvailability(GitBinaryPath);
	}

	// 4) Else, look for the version of git provided by Command Line Tools
	if (!bFound)
	{
		GitBinaryPath = TEXT("/usr/bin/git");
		bFound = CheckGitAvailability(GitBinaryPath);
	}

	{
		SCOPED_AUTORELEASE_POOL;
		NSWorkspace* SharedWorkspace = [NSWorkspace sharedWorkspace];

		// 5) Else, look for the version of local_git provided by SmartGit
		if (!bFound)
		{
			NSURL* AppURL = [SharedWorkspace URLForApplicationWithBundleIdentifier : @"com.syntevo.smartgit"];
			if (AppURL != nullptr)
			{
				NSBundle* Bundle = [NSBundle bundleWithURL : AppURL];
				GitBinaryPath = FString::Printf(TEXT("%s/git/bin/git"), *FString([Bundle resourcePath]));
				bFound = CheckGitAvailability(GitBinaryPath);
			}
		}

		// 6) Else, look for the version of local_git provided by SourceTree
		if (!bFound)
		{
			NSURL* AppURL = [SharedWorkspace URLForApplicationWithBundleIdentifier : @"com.torusknot.SourceTreeNotMAS"];
			if (AppURL != nullptr)
			{
				NSBundle* Bundle = [NSBundle bundleWithURL : AppURL];
				GitBinaryPath = FString::Printf(TEXT("%s/git_local/bin/git"), *FString([Bundle resourcePath]));
				bFound = CheckGitAvailability(GitBinaryPath);
			}
		}

		// 7) Else, look for the version of local_git provided by GitHub Desktop
		if (!bFound)
		{
			NSURL* AppURL = [SharedWorkspace URLForApplicationWithBundleIdentifier : @"com.github.GitHubClient"];
			if (AppURL != nullptr)
			{
				NSBundle* Bundle = [NSBundle bundleWithURL : AppURL];
				GitBinaryPath = FString::Printf(TEXT("%s/app/git/bin/git"), *FString([Bundle resourcePath]));
				bFound = CheckGitAvailability(GitBinaryPath);
			}
		}

		// 8) Else, look for the version of local_git provided by Tower2
		if (!bFound)
		{
			NSURL* AppURL = [SharedWorkspace URLForApplicationWithBundleIdentifier : @"com.fournova.Tower2"];
			if (AppURL != nullptr)
			{
				NSBundle* Bundle = [NSBundle bundleWithURL : AppURL];
				GitBinaryPath = FString::Printf(TEXT("%s/git/bin/git"), *FString([Bundle resourcePath]));
				bFound = CheckGitAvailability(GitBinaryPath);
			}
		}
	}

#else
	FString GitBinaryPath = TEXT("/usr/bin/git");
	bool bFound = CheckGitAvailability(GitBinaryPath);
#endif

	if (bFound)
	{
		FPaths::MakePlatformFilename(GitBinaryPath);
	}
	else
	{
		// If we did not find a path to Git, set it empty
		GitBinaryPath.Empty();
	}

	return GitBinaryPath;
}

bool GitPushUtils::GetBranchName(const FString& InPathToGitBinary, const FString& InRepositoryRoot, FString& OutBranchName)
{
	bool bResults;
	FString InfoMessage;
	FString ErrorMessage;
	TArray<FString> Parameters;
	Parameters.Add(TEXT("--short"));
	Parameters.Add(TEXT("--quiet"));		// no error message while in detached HEAD
	Parameters.Add(TEXT("HEAD"));
	bResults = RunCommand(TEXT("symbolic-ref"), InPathToGitBinary, InRepositoryRoot, Parameters, TArray<FString>(), InfoMessage, ErrorMessage);
	TArray<FString> InfoMessages;
	InfoMessage.ParseIntoArray(InfoMessages, TEXT("\n"));
	if (bResults && InfoMessages.Num() > 0)
	{
		OutBranchName = InfoMessages[0];
	}
	else
	{
		Parameters.Reset();
		Parameters.Add(TEXT("-1"));
		Parameters.Add(TEXT("--format=\"%h\""));		// no error message while in detached HEAD
		bResults = RunCommand(TEXT("log"), InPathToGitBinary, InRepositoryRoot, Parameters, TArray<FString>(), InfoMessage, ErrorMessage);
		InfoMessage.ParseIntoArray(InfoMessages, TEXT("\n"));
		if (bResults && InfoMessages.Num() > 0)
		{
			OutBranchName = "HEAD detached at ";
			OutBranchName += InfoMessages[0];
		}
		else
		{
			bResults = false;
		}
	}

	return bResults;
}

FString GitPushUtils::RunPush() {
	bool bResult;
	FString Results;
	FString Errors;
	FString RootDirectory;
	FString CurrentBranch;
	FString GitBinary = FindGitBinaryPath();
	FindRootDirectory(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()), RootDirectory);
	GetBranchName(GitBinary, RootDirectory, CurrentBranch);

	TArray<FString> Parameters = TArray<FString>();
	Parameters.Add(TEXT("origin"));
	Parameters.Add(CurrentBranch);

	bResult = RunCommand("push", GitBinary, RootDirectory, Parameters, TArray<FString>(), Results, Errors);

	if (!bResult) {
		UE_LOG(LogGitPushModule, Log, TEXT("Error: %s"), *Errors);
		return Errors;
	}
	UE_LOG(LogGitPushModule, Log, TEXT("%s"), *Results);
	return "None";
}
