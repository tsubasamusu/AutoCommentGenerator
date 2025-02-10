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
 	const TArray<UEdGraphNode*> ActiveNodesUnderThisComment = GetActiveNodes(GetNodesUnderThisComment());
 
 	if (ActiveNodesUnderThisComment.Num() == 0) return FString();

	const TArray<FNodeData> NodesData = GetNodesData(ActiveNodesUnderThisComment);

	FString JsonString;

	if (!FJsonObjectConverter::UStructToJsonObjectString(FNodesData(NodesData), JsonString, 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize the json object."));
	}

	return JsonString;
}

TArray<FNodeData> SEnhancedCommentNode::GetNodesData(const TArray<UEdGraphNode*>& InNodes)
{
	TArray<FNodeData> NodesData;

	for (UEdGraphNode* Node : InNodes)
	{
		FNodeData NodeData;

		NodeData.Pins = GetPinsData(Node);
		NodeData.Comment = Node->NodeComment;

		NodesData.Add(NodeData);
	}

	return NodesData;
}

TArray<FPinData> SEnhancedCommentNode::GetPinsData(const UEdGraphNode* InNode)
{
	TArray<FPinData> PinsData;

	for (UEdGraphPin* Pin : InNode->Pins)
	{
		// ignore pins that are not connected anywhere
		if (Pin->LinkedTo.Num() == 0) continue;

		FPinData PinData;

		PinData.PinName = Pin->GetDisplayName().IsEmpty() ? Pin->PinName.ToString() : Pin->GetDisplayName().ToString();
		PinData.PinId = Pin->PinId.ToString();
		PinData.IdsOfPinsConnectedToMyself = GetPinIds(Pin->LinkedTo);
		PinData.PinType = GetPinTypeAsString(Pin);

		PinsData.Add(PinData);
	}

	return PinsData;
}

TArray<FString> SEnhancedCommentNode::GetPinIds(const TArray<UEdGraphPin*>& InPins)
{
	TArray<FString> PinIds;

	for (UEdGraphPin* Pin : InPins)
	{
		PinIds.Add(Pin->PinId.ToString());
	}

	return PinIds;
}

FString SEnhancedCommentNode::GetPinTypeAsString(const UEdGraphPin* InPin)
{
	switch (InPin->Direction)
	{
	case EEdGraphPinDirection::EGPD_Input:
		return TEXT("Input");
	case EEdGraphPinDirection::EGPD_Output:
		return TEXT("Output");
	default:
		return TEXT("UnknownType");
	}
}

TArray<UEdGraphNode*> SEnhancedCommentNode::GetActiveNodes(const TArray<UEdGraphNode*>& InNodes)
{
	TArray<UEdGraphNode*> ActiveNodes;

	for (UEdGraphNode* Node : InNodes)
	{
		if (!HasAnyConnectedPins(Node)) continue;

		ActiveNodes.Add(Node);
	}

	return ActiveNodes;
}

bool SEnhancedCommentNode::HasAnyConnectedPins(const UEdGraphNode* InNode)
{
	return GetPinsData(InNode).Num() > 0;
}