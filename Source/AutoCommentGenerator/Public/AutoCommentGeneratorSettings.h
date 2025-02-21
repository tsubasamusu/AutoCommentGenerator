// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/EditorSettings.h"
#include "AutoCommentGeneratorSettings.generated.h"

UCLASS(config = EditorPerProjectUserSettings)
class UAutoCommentGeneratorSettings final : public UObject
{
	GENERATED_BODY()

public:
	UAutoCommentGeneratorSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, config, Category = GPT, meta = (DisplayName = "Open AI API Key"))
	FString ApiKey;
};