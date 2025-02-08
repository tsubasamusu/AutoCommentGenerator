// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "EnhancedCommentNode.h"

void SEnhancedCommentNode::Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode)
{
	Super::Construct(SGraphNodeComment::FArguments(), InNode);
}

void SEnhancedCommentNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SEnhancedCommentNode::SetComment(const FString& NewComment)
{
	//TODO:日本語も設定可能にする

	OnCommentTextCommitted(FText::FromString(NewComment), ETextCommit::Type::Default);
}