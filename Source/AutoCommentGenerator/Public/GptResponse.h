// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GptResponse.generated.h"

USTRUCT()
struct FGptResponse
{
	GENERATED_BODY()
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