// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GptRequest.generated.h"

USTRUCT()
struct AUTOCOMMENTGENERATOR_API FGptMessage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString role;

	UPROPERTY()
	FString content;
};

USTRUCT()
struct AUTOCOMMENTGENERATOR_API FGptRequest
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString model;

	UPROPERTY()
	TArray<FGptMessage> messages;
};