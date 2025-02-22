// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorSettings.h"
#include "Internationalization/Culture.h"

UAutoCommentGeneratorSettings::UAutoCommentGeneratorSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ApiKey = TEXT("");
	GptLanguageCultureName = FInternationalization::Get().GetCurrentLanguage()->GetName();
	CommentGenerationConditions = { TEXT("answer briefly") };
	bIgnoreNodesDoNotHaveConnectedPins = true;
	bIgnoreCommentNodes = false;
	ButtonTopPadding = 10.f;
	ButtonRightPadding = 10.f;
	ButtonColor = FLinearColor::White;
	ButtonSize = FVector2D(15, 15);
}

FCulturePtr UAutoCommentGeneratorSettings::GetGptLanguageCulture() const
{
	return FInternationalization::Get().GetCulture(GptLanguageCultureName);
}

void UAutoCommentGeneratorSettings::SetGptLanguageCulture(const FCulturePtr& InGptLanguageCulture)
{
	GptLanguageCultureName = InGptLanguageCulture->GetName();

	SaveConfig();
}