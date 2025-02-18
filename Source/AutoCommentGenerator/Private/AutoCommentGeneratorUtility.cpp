// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorUtility.h"
#include "EdGraphNode_Comment.h"
#include "JsonObjectConverter.h"
#include "EdGraph/EdGraphPin.h"
#include "NodesData.h"
#include "Styling/AppStyle.h"

TArray<FNodeData> FAutoCommentGeneratorUtility::GetNodesData(const TArray<UEdGraphNode*>& InNodes)
{
	TArray<FNodeData> NodesData;

	for (UEdGraphNode* Node : InNodes)
	{
		FNodeData NodeData;

		NodeData.NodeName = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
		NodeData.Comment = Node->NodeComment;
		NodeData.bIsCommentNode = IsCommentNode(Node);
		NodeData.Pins = GetPinsData(Node);

		NodesData.Add(NodeData);
	}

	return NodesData;
}

TArray<FPinData> FAutoCommentGeneratorUtility::GetPinsData(const UEdGraphNode* InNode)
{
	TArray<FPinData> PinsData;

	for (UEdGraphPin* Pin : InNode->GetAllPins())
	{
		FPinData PinData;

		PinData.PinName = Pin->GetDisplayName().IsEmpty() ? Pin->PinName.ToString() : Pin->GetDisplayName().ToString();
		PinData.PinType = GetPinTypeAsString(Pin);
		PinData.PinId = Pin->PinId.ToString();
		PinData.DefaultValue = Pin->GetDefaultAsString();
		PinData.bDefaultValueIsUsed = IsPinUsesDefaultValue(Pin);
		PinData.ConnectedPinIds = GetPinIds(Pin->LinkedTo);

		PinsData.Add(PinData);
	}

	return PinsData;
}

TArray<FString> FAutoCommentGeneratorUtility::GetPinIds(const TArray<UEdGraphPin*>& InPins)
{
	TArray<FString> PinIds;

	for (UEdGraphPin* Pin : InPins)
	{
		PinIds.Add(Pin->PinId.ToString());
	}

	return PinIds;
}

FString FAutoCommentGeneratorUtility::GetPinTypeAsString(const UEdGraphPin* InPin)
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

TArray<UEdGraphNode*> FAutoCommentGeneratorUtility::GetActiveNodes(const TArray<UEdGraphNode*>& InNodes)
{
	TArray<UEdGraphNode*> ActiveNodes;

	for (UEdGraphNode* Node : InNodes)
	{
		// ignore nodes that do not have connected pins and are not comment nodes
		if (!HasConnectedPins(Node) && !IsCommentNode(Node)) continue;

		ActiveNodes.Add(Node);
	}

	return ActiveNodes;
}

bool FAutoCommentGeneratorUtility::HasConnectedPins(const UEdGraphNode* InNode)
{
	TArray<UEdGraphPin*> Pins = InNode->GetAllPins();

	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->HasAnyConnections()) return true;
	}

	return false;
}

bool FAutoCommentGeneratorUtility::IsCommentNode(const UEdGraphNode* InNode)
{
	return IsValid(Cast<UEdGraphNode_Comment>(InNode));
}

bool FAutoCommentGeneratorUtility::IsPinUsesDefaultValue(const UEdGraphPin* InPin)
{
	return !InPin->HasAnyConnections() && InPin->Direction == EEdGraphPinDirection::EGPD_Input;
}

int32 FAutoCommentGeneratorUtility::GetCharNum(const FString& InString, const TCHAR& InChar)
{
	int32 CharNum = 0;

	for (const TCHAR Char : InString)
	{
		if (Char == InChar)
		{
			CharNum++;
		}
	}

	return CharNum;
}

const FSlateBrush* FAutoCommentGeneratorUtility::GetPlayIcon()
{
	return FAppStyle::Get().GetBrush("Animation.Forward");
}

const FSlateBrush* FAutoCommentGeneratorUtility::GetStopIcon()
{
	return FAppStyle::Get().GetBrush("Animation.Pause");
}