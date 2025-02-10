// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNodeComment.h"

class UEdGraphNode_Comment;
struct FNodeData;
struct FPinData;

class SEnhancedCommentNode final : public SGraphNodeComment
{
	typedef SGraphNodeComment Super;

public:
	SLATE_BEGIN_ARGS(SEnhancedCommentNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	void SetComment(const FString& NewComment);

	TArray<UEdGraphNode*> GetNodesUnderThisComment();

	FString GetNodesDataUnderThisCommentAsJsonString();

	static TArray<FNodeData> GetNodesData(const TArray<UEdGraphNode*>& InNodes);

	static TArray<FPinData> GetPinsData(const UEdGraphNode* InNode);
	
	static TArray<FString> GetPinIds(const TArray<UEdGraphPin*>& InPins);

	static FString GetPinTypeAsString(const UEdGraphPin* InPin);

	static TArray<UEdGraphNode*> GetActiveNodes(const TArray<UEdGraphNode*>& InNodes);
	
	static bool HasAnyConnectedPins(const UEdGraphNode* InNode);
};