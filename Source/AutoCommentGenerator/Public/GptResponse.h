// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GptRequest.h"
#include "GptResponse.generated.h"

USTRUCT()
struct FGptResponseChoice
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGptMessage message;
};

USTRUCT()
struct FGptResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FGptResponseChoice> choices;

	bool IsEmpty() const
	{
		if (choices.Num() == 0) return true;

		if (choices[0].message.content.IsEmpty()) return true;

		return false;
	}

	FString GetGptMessage()const
	{
		if (IsEmpty()) return FString();

		return choices[0].message.content;
	}
};

USTRUCT()
struct FGptError
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString message;

	UPROPERTY()
	FString type;

	UPROPERTY()
	FString code;
};

USTRUCT()
struct FGptErrorResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGptError error;

	bool IsEmpty() const
	{
		return error.message.IsEmpty() && error.type.IsEmpty() && error.code.IsEmpty();
	}
};