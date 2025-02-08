// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorModule.h"
#include "Misc/CoreDelegates.h"
#include "EdGraphUtilities.h"
#include "EnhancedCommentNodeFactory.h"

#define LOCTEXT_NAMESPACE "FAutoCommentGeneratorModule"

#define ACG_IS_ENABLED (!IS_MONOLITHIC && !UE_BUILD_SHIPPING && !UE_BUILD_TEST && !UE_GAME && !UE_SERVER)

void FAutoCommentGeneratorModule::StartupModule()
{
#if ACG_IS_ENABLED
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FAutoCommentGeneratorModule::RegisterCommentNodeFactory);
#endif
}

void FAutoCommentGeneratorModule::ShutdownModule()
{
#if ACG_IS_ENABLED
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAutoCommentGeneratorModule, AutoCommentGenerator)