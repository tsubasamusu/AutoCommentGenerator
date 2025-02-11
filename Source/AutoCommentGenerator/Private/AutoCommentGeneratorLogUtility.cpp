// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorLogUtility.h"

DEFINE_LOG_CATEGORY(AutoCommentGeneratorLog);

void FAutoCommentGeneratorLogUtility::Log(const FString& InMessage)
{
	UE_LOG(AutoCommentGeneratorLog, Log, TEXT("%s"), *InMessage);
}

void FAutoCommentGeneratorLogUtility::LogError(const FString& InMessage)
{
	UE_LOG(AutoCommentGeneratorLog, Error, TEXT("%s"), *InMessage);
}

void FAutoCommentGeneratorLogUtility::LogWarning(const FString& InMessage)
{
	UE_LOG(AutoCommentGeneratorLog, Warning, TEXT("%s"), *InMessage);
}