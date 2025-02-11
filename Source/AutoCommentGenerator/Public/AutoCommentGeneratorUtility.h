// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"

struct FNodeData;
struct FPinData;
class UEdGraphNode;
class UEdGraphPin;

struct FAutoCommentGeneratorUtility
{
	static TArray<FNodeData> GetNodesData(const TArray<UEdGraphNode*>& InNodes);

	static TArray<FPinData> GetPinsData(const UEdGraphNode* InNode);

	static TArray<FString> GetPinIds(const TArray<UEdGraphPin*>& InPins);

	static FString GetPinTypeAsString(const UEdGraphPin* InPin);

	static TArray<UEdGraphNode*> GetActiveNodes(const TArray<UEdGraphNode*>& InNodes);

	static bool HasConnectedPins(const UEdGraphNode* InNode);

	static bool IsCommentNode(const UEdGraphNode* InNode);

	static bool IsPinUsesDefaultValue(const UEdGraphPin* InPin);
};