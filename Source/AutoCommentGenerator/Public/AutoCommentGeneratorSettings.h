// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/EditorSettings.h"
#include "AutoCommentGeneratorSettings.generated.h"

UCLASS(config = Editor, defaultconfig)
class UAutoCommentGeneratorSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = GPT, meta = (DisplayName = "Open AI API Key"))
	FString ApiKey;
};