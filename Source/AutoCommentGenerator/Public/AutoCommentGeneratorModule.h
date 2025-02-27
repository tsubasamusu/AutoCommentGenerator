// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class ISettingsModule;
class FEnhancedCommentNodeFactory;

class AUTOCOMMENTGENERATOR_API FAutoCommentGeneratorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterCommentNodeFactory();
	void UnregisterCommentNodeFactory();

	void RegisterSettings();
	void UnregisterSettings();

	static ISettingsModule* GetSettingsModuleChecked();

	const FName SettingsContainerName = TEXT("Editor");
	const FName SettingsCategoryName = TEXT("Plugins");
	const FName SettingsSectionName = TEXT("Auto Comment Generator");

	TSharedPtr<FEnhancedCommentNodeFactory> EnhancedCommentNodeFactoryPtr;
};