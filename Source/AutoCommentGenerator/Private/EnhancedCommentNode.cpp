// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "EnhancedCommentNode.h"
#include "EdGraphNode_Comment.h"
#include "JsonObjectConverter.h"
#include "EdGraph/EdGraphPin.h"
#include "NodesData.h"

void SEnhancedCommentNode::Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode)
{
	Super::Construct(SGraphNodeComment::FArguments(), InNode);
}

void SEnhancedCommentNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// debug
	UE_LOG(LogTemp, Log, TEXT("%s"), *GetNodesDataUnderThisCommentAsJsonString());
}

void SEnhancedCommentNode::SetComment(const FString& NewComment)
{
	//TODO:enable Japanese input too

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

	// reconfirm nodes under this comment
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

FString SEnhancedCommentNode::GetNodesDataUnderThisCommentAsJsonString()
{
 	TArray<UEdGraphNode*> NodesUnderThisComment = GetNodesUnderThisComment();
 
 	if (NodesUnderThisComment.Num() == 0) return FString();

	FNodesData NodesData = FNodesData(GetNodesData(NodesUnderThisComment));

	FString JsonString;

	if (!FJsonObjectConverter::UStructToJsonObjectString(NodesData, JsonString, 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize the json object."));
	}

	return JsonString;
}

TArray<FNodeData> SEnhancedCommentNode::GetNodesData(const TArray<UEdGraphNode*>& SourceNodes)
{
	TArray<FNodeData> NodesData;

	for (UEdGraphNode* SourceNode : SourceNodes)
	{
		FNodeData NodeData;

		NodeData.Pins = GetPinsData(SourceNode);

		NodesData.Add(NodeData);
	}

	return NodesData;
}

TArray<FPinData> SEnhancedCommentNode::GetPinsData(const UEdGraphNode* SourceNode)
{
	TArray<FPinData> PinsData;

	for (UEdGraphPin* Pin : SourceNode->Pins)
	{
		FPinData PinData;

		PinData.PinName = Pin->PinName.ToString();

		PinsData.Add(PinData);
	}

	return PinsData;
}