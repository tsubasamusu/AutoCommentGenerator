// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"

class FCommentGenerator
{
public:
	static void GenerateComment(const FString& NodesDataString, const TFunction<void(const bool bSucceeded, const FString& Message)>& OnGeneratedComment);

private:
	static bool TryGetGptRequestString(const FString& NodesDataString, FString& OutGptRequestString);

	static FString GetDesiredPrompt(const FString& NodesDataString);
};