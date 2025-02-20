// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "CommentGenerator.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "GptRequest.h"
#include "GptResponse.h"

void FCommentGenerator::GenerateComment(const FString& NodesDataString, const TFunction<void(bool bSucceeded, const FString& Message)>& OnGeneratedComment)
{
	FString GptRequestString;

	if (!TryGetGptRequestAsJsonString(NodesDataString, GptRequestString))
	{
		OnGeneratedComment(false, TEXT("Failed to create a GPT request."));

		return;
	}

	const FString ApiKey = TEXT("");

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

bool FCommentGenerator::TryGetGptRequestAsJsonString(const FString& NodesDataString, FString& OutGptRequestString)
{
	FGptRequest GptRequest =
	{
		.model = TEXT("gpt-3.5-turbo"),
		.messages =
		{
			{
				.role = TEXT("system"),
				.content = TEXT("You are a programmer using the Unreal Engine and specialize in writing comments for comment nodes. Other programmers submit JSON strings of blueprints to you, and you predict what they will do and return the appropriate comments.")
			},
			{
				.role = TEXT("user"),
				.content = NodesDataString
			}
		}
	};

	return FJsonObjectConverter::UStructToJsonObjectString(GptRequest, OutGptRequestString, 0, 0);
}