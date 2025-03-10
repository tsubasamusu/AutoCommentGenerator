// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorModule.h"
#include "AutoCommentGeneratorSettings.h"
#include "AutoCommentGeneratorSettingsCustomization.h"
#include "EdGraphUtilities.h"
#include "EnhancedCommentNodeFactory.h"
#include "ISettingsModule.h"
#include "Misc/CoreDelegates.h"
#include "PropertyEditorModule.h"

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

void FAutoCommentGeneratorModule::RegisterSettings() const
{
	const FText SettingsDisplayName = LOCTEXT("SettingsDisplayName", "Auto Comment Generator");
	const FText SettingsDescription = LOCTEXT("SettingsDescription", "Configure the Auto Comment Generator plugin");

	GetSettingsModuleChecked()->RegisterSettings(SettingsContainerName, SettingsCategoryName, SettingsSectionName, SettingsDisplayName, SettingsDescription, GetMutableDefault<UAutoCommentGeneratorSettings>());

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	const FName SettingsClassName = UAutoCommentGeneratorSettings::StaticClass()->GetFName();

	PropertyModule.RegisterCustomClassLayout(SettingsClassName, FOnGetDetailCustomizationInstance::CreateStatic(&FAutoCommentGeneratorSettingsCustomization::Create));
}

void FAutoCommentGeneratorModule::UnregisterSettings() const
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