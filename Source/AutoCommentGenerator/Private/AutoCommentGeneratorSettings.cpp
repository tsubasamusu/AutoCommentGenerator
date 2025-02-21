// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorSettings.h"

UAutoCommentGeneratorSettings::UAutoCommentGeneratorSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ButtonTopPadding = 10.f;
	ButtonRightPadding = 10.f;
	ButtonColor = FLinearColor::White;
	ButtonSize = FVector2D(15, 15);
}