// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "EnhancedCommentNode.h"
#include "EdGraphNode_Comment.h"

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
	//TODO:ì˙ñ{åÍÇ‡ê›íËâ¬î\Ç…Ç∑ÇÈ

	OnCommentTextCommitted(FText::FromString(NewComment), ETextCommit::Type::Default);
}

TArray<UEdGraphNode*> SEnhancedCommentNode::GetNodesUnderThisComment()
{
	UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(GetNodeObj());

	if (!IsValid(CommentNode))
	{
		UE_LOG(LogTemp, Error, TEXT("Type of the node being observed by SEnhancedCommentNode is not UEdGraphNode_Comment."));

		return TArray<UEdGraphNode*>();
	}

	// Reconfirm nodes under this comment.
	HandleSelection(true, true);

	TArray<UEdGraphNode*> NodesUnderThisComment;

	for (UObject* NodeObject : CommentNode->GetNodesUnderComment())
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(NodeObject))
		{
			NodesUnderThisComment.Add(Node);
		}
	}

	return NodesUnderThisComment;
}