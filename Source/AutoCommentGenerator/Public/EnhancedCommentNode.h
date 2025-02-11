// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNodeComment.h"

class SEnhancedCommentNode : public SGraphNodeComment
{
	typedef SGraphNodeComment Super;

public:
	SLATE_BEGIN_ARGS(SEnhancedCommentNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void SetComment(const FString& NewComment);

	TArray<UEdGraphNode*> GetNodesUnderThisComment();

	FString GetNodesDataUnderThisCommentAsJsonString();

	bool TryGetTitleBarSize(FVector2D& OutTitleBarSize);

protected:
	virtual void UpdateGraphNode() override;
};