// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "CommentGenerator.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "GptRequest.h"
#include "GptResponse.h"
#include "AutoCommentGeneratorUtility.h"
#include "AutoCommentGeneratorSettings.h"
#include "Internationalization/Culture.h"

void FCommentGenerator::GenerateComment(const FString& NodesDataString, const TFunction<void(const bool bSucceeded, const FString& Message)>& OnGeneratedComment)
{
	FString GptRequestString;

	if (!TryGetGptRequestString(NodesDataString, GptRequestString))
	{
		OnGeneratedComment(false, TEXT("Failed to create a GPT request."));

		return;
	}

	const FString ApiKey = FAutoCommentGeneratorUtility::GetSettingsChecked()->ApiKey;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + ApiKey);
	HttpRequest->SetContentAsString(GptRequestString);

	HttpRequest->OnProcessRequestComplete().BindLambda([OnGeneratedComment](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSucceeded)
		{
			if (!bSucceeded)
			{
				OnGeneratedComment(false, TEXT("Failed to send a HTTP request."));

				return;
			}

			if (!HttpResponsePtr.IsValid())
			{
				OnGeneratedComment(false, TEXT("Failed to get a HTTP response."));

				return;
			}

			FString JsonResponse = HttpResponsePtr->GetContentAsString();

			FGptErrorResponse GptErrorResponse;

			if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonResponse, &GptErrorResponse, 0, 0) && !GptErrorResponse.IsEmpty())
			{
				OnGeneratedComment(false, GptErrorResponse.error.message);

				return;
			}

			FGptResponse GptResponse;

			if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonResponse, &GptResponse, 0, 0))
			{
				OnGeneratedComment(false, TEXT("Failed to get a GPT response."));

				return;
			}

			if (GptResponse.IsEmpty())
			{
				OnGeneratedComment(false, TEXT("Failed to get a GPT response."));

				return;
			}

			OnGeneratedComment(true, GptResponse.GetGptMessage());
		});

	if (!HttpRequest->ProcessRequest())
	{
		OnGeneratedComment(false, TEXT("Failed to process a HTTP request."));
	}
}

bool FCommentGenerator::TryGetGptRequestString(const FString& NodesDataString, FString& OutGptRequestString)
{
	FGptRequest GptRequest =
	{
		.model = TEXT("gpt-3.5-turbo"),
		.messages =
		{
			{
				.role = TEXT("user"),
				.content = GetDesiredPrompt(NodesDataString)
			}
		}
	};

	return FJsonObjectConverter::UStructToJsonObjectString(GptRequest, OutGptRequestString, 0, 0);
}

FString FCommentGenerator::GetDesiredPrompt(const FString& NodesDataString)
{
	UAutoCommentGeneratorSettings* Settings = FAutoCommentGeneratorUtility::GetSettingsChecked();

	FString Prompt = TEXT("You are developing a game using the Unreal Engine and are going to write a comment in a comment node for a blueprint process represented by the following string in JSON format. Answer the appropriate comment to be written in the comment node according to the following conditions.");

	Prompt += TEXT("\n\n- answer in ") + Settings->GetGptLanguageCulture()->GetEnglishName();

	for (FString CommentGenerationCondition : Settings->CommentGenerationConditions)
	{
		Prompt += TEXT("\n- ") + CommentGenerationCondition;
	}

	Prompt += TEXT("\n\n") + NodesDataString;

	return Prompt;
}