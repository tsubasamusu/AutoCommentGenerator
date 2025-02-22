// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/EditorSettings.h"
#include "AutoCommentGeneratorSettings.generated.h"

struct FLinearColor;

UCLASS(config = EditorPerProjectUserSettings)
class UAutoCommentGeneratorSettings final : public UObject
{
	GENERATED_BODY()

public:
	UAutoCommentGeneratorSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, config, Category = GPT, meta = (DisplayName = "Open AI API Key"))
	FString ApiKey;
	
	UPROPERTY(VisibleAnywhere, config, Category = GPT, meta = (DisplayName = "Culture Name of GPT Language"))
	FString GptLanguageCultureName;
	
	UPROPERTY(EditAnywhere, config, Category = GPT)
	TArray<FString> CommentGenerationConditions;

	UPROPERTY(EditAnywhere, config, Category = Behavior)
	bool bIgnoreNodesDoNotHaveConnectedPins;
	
	UPROPERTY(EditAnywhere, config, Category = Behavior)
	bool bIgnoreCommentNodes;

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