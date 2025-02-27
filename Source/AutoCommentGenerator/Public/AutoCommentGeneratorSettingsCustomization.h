// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;

class AUTOCOMMENTGENERATOR_API FAutoCommentGeneratorSettingsCustomization final : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> Create();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

private:
	void ChangeApiKeyPropertyDisplayAsPassword(IDetailLayoutBuilder& DetailLayout);

	void AddGptLanguageProperty(IDetailLayoutBuilder& DetailLayout);

	void AddGptModelsDocumentButton(IDetailLayoutBuilder& DetailLayout);
};