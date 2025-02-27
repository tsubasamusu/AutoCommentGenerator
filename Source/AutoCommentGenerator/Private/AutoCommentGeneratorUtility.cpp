// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorUtility.h"
#include "EdGraphNode_Comment.h"
#include "JsonObjectConverter.h"
#include "EdGraph/EdGraphPin.h"
#include "NodesData.h"
#include "Styling/AppStyle.h"
#include "AutoCommentGeneratorSettings.h"

TArray<FNodeData> FAutoCommentGeneratorUtility::GetNodesData(const TArray<UEdGraphNode*>& InNodes)
{
	TArray<FNodeData> NodesData;

	for (const UEdGraphNode* Node : InNodes)
	{
		FNodeData NodeData =
		{
			.NodeName = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString(),
			.Comment = Node->NodeComment,
			.bIsCommentNode = IsCommentNode(Node),
			.Pins = GetPinsData(Node)
		};

		NodesData.Add(NodeData);
	}

	return NodesData;
}

TArray<FPinData> FAutoCommentGeneratorUtility::GetPinsData(const UEdGraphNode* InNode)
{
	TArray<FPinData> PinsData;

	for (const UEdGraphPin* Pin : InNode->GetAllPins())
	{
		FPinData PinData
		{
			.PinName = Pin->GetDisplayName().IsEmpty() ? Pin->PinName.ToString() : Pin->GetDisplayName().ToString(),
			.PinType = GetPinTypeAsString(Pin),
			.PinId = Pin->PinId.ToString(),
			.DefaultValue = Pin->GetDefaultAsString(),
			.bDefaultValueIsUsed = IsPinUsesDefaultValue(Pin),
			.ConnectedPinIds = GetPinIds(Pin->LinkedTo)
		};

		PinsData.Add(PinData);
	}

	return PinsData;
}

TArray<FString> FAutoCommentGeneratorUtility::GetPinIds(const TArray<UEdGraphPin*>& InPins)
{
	TArray<FString> PinIds;

	for (const UEdGraphPin* Pin : InPins)
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
	const UAutoCommentGeneratorSettings* Settings = GetSettingsChecked();

	TArray<UEdGraphNode*> ActiveNodes;

	for (UEdGraphNode* Node : InNodes)
	{
		if (HasConnectedPins(Node))
		{
			ActiveNodes.Add(Node);

			continue;
		}

		if (IsCommentNode(Node))
		{
			if (!Settings->bIgnoreCommentNodes)
			{
				ActiveNodes.Add(Node);
			}

			continue;
		}

		if (!Settings->bIgnoreNodesDoNotHaveConnectedPins)
		{
			ActiveNodes.Add(Node);
		}
	}

	return ActiveNodes;
}

bool FAutoCommentGeneratorUtility::HasConnectedPins(const UEdGraphNode* InNode)
{
	for (TArray<UEdGraphPin*> Pins = InNode->GetAllPins(); const UEdGraphPin* Pin : Pins)
	{
		if (Pin->HasAnyConnections())
		{
			return true;
		}
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
	return FAppStyle::Get().GetBrush(TEXT("Animation.Forward"));
}

const FSlateBrush* FAutoCommentGeneratorUtility::GetStopIcon()
{
	return FAppStyle::Get().GetBrush(TEXT("Animation.Pause"));
}

UAutoCommentGeneratorSettings* FAutoCommentGeneratorUtility::GetSettingsChecked()
{
	UAutoCommentGeneratorSettings* Settings = GetMutableDefault<UAutoCommentGeneratorSettings>();

	check(Settings);

	return Settings;
}