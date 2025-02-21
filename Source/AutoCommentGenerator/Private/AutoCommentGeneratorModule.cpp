// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorModule.h"
#include "Misc/CoreDelegates.h"
#include "EdGraphUtilities.h"
#include "EnhancedCommentNodeFactory.h"
#include "AutoCommentGeneratorSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FAutoCommentGeneratorModule"

#define ACG_IS_ENABLED (!IS_MONOLITHIC && !UE_BUILD_SHIPPING && !UE_BUILD_TEST && !UE_GAME && !UE_SERVER)

void FAutoCommentGeneratorModule::StartupModule()
{
#if ACG_IS_ENABLED

	RegisterSettings();

	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FAutoCommentGeneratorModule::RegisterCommentNodeFactory);

#endif
}

void FAutoCommentGeneratorModule::ShutdownModule()
{
#if ACG_IS_ENABLED

	UnregisterSettings();

	UnregisterCommentNodeFactory();

#endif
}

void FAutoCommentGeneratorModule::RegisterCommentNodeFactory()
{
	EnhancedCommentNodeFactoryPtr = MakeShared<FEnhancedCommentNodeFactory>();

	FEdGraphUtilities::RegisterVisualNodeFactory(EnhancedCommentNodeFactoryPtr);
}

void FAutoCommentGeneratorModule::UnregisterCommentNodeFactory()
{
	FEdGraphUtilities::UnregisterVisualNodeFactory(EnhancedCommentNodeFactoryPtr);

	EnhancedCommentNodeFactoryPtr.Reset();
}

void FAutoCommentGeneratorModule::RegisterSettings()
{
	const FText SettingsDisplayName = LOCTEXT("SettingsDisplayName", "Auto Comment Generator");
	const FText SettingsDescription = LOCTEXT("SettingsDescription", "Configure the Auto Comment Generator plugin");

	GetSettingsModuleChecked()->RegisterSettings(SettingsContainerName, SettingsCategoryName, SettingsSectionName, SettingsDisplayName, SettingsDescription, GetMutableDefault<UAutoCommentGeneratorSettings>());
}

void FAutoCommentGeneratorModule::UnregisterSettings()
{
	GetSettingsModuleChecked()->UnregisterSettings(SettingsContainerName, SettingsCategoryName, SettingsSectionName);
}

ISettingsModule* FAutoCommentGeneratorModule::GetSettingsModuleChecked()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"));

	check(SettingsModule);

	return SettingsModule;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAutoCommentGeneratorModule, AutoCommentGenerator)