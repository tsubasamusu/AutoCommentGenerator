// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FEnhancedCommentNodeFactory;

class FAutoCommentGeneratorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterCommentNodeFactory();
	void UnregisterCommentNodeFactory();

	TSharedPtr<FEnhancedCommentNodeFactory> EnhancedCommentNodeFactoryPtr;
};