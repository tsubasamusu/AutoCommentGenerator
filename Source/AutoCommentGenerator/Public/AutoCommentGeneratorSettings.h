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
	
	UPROPERTY(EditAnywhere, config, Category = GPT, meta = (DisplayName = "GPT Model Name"))
	FString GptModelName;
	
	UPROPERTY(VisibleAnywhere, config, Category = GPT, meta = (DisplayName = "Culture Name of GPT Language"))
	FString GptLanguageCultureName;
	
	/* Whether to ignore nodes that have no input pins, output pins, execution pins, etc. connected to them when generating comments */
	UPROPERTY(EditAnywhere, config, Category = Behavior)
	bool bIgnoreNodesDoNotHaveConnectedPins;
	
	/* Whether to ignore comment nodes contained within a comment node when generating comments */
	UPROPERTY(EditAnywhere, config, Category = Behavior)
	bool bIgnoreCommentNodes;
	
	/* Conditions that AI must adhere to when generating comments */
	UPROPERTY(EditAnywhere, config, Category = Behavior)
	TArray<FString> CommentGenerationConditions;

	UPROPERTY(EditAnywhere, config, Category = Appearance, meta = (DisplayName = "Generate Comment Button Top Padding"))
	float ButtonTopPadding;

	UPROPERTY(EditAnywhere, config, Category = Appearance, meta = (DisplayName = "Generate Comment Button Right Padding"))
	float ButtonRightPadding;

	UPROPERTY(EditAnywhere, config, Category = Appearance, meta = (DisplayName = "Generate Comment Button Color"))
	FLinearColor ButtonColor;

	UPROPERTY(EditAnywhere, config, Category = Appearance, meta = (DisplayName = "Generate Comment Button Size"))
	FVector2D ButtonSize;

	FCulturePtr GetGptLanguageCulture() const;

	void SetGptLanguageCulture(const FCulturePtr& InGptLanguageCulture);
};