// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"

struct FNodeData;
struct FPinData;
struct FSlateBrush;
class UEdGraphNode;
class UEdGraphPin;
class UAutoCommentGeneratorSettings;

class AUTOCOMMENTGENERATOR_API FAutoCommentGeneratorUtility
{
public:
	static TArray<FNodeData> GetNodesData(const TArray<UEdGraphNode*>& InNodes);

	static TArray<FPinData> GetPinsData(const UEdGraphNode* InNode);

	static TArray<FString> GetPinIds(const TArray<UEdGraphPin*>& InPins);

	static FString GetPinTypeAsString(const UEdGraphPin* InPin);

	static TArray<UEdGraphNode*> GetActiveNodes(const TArray<UEdGraphNode*>& InNodes);

	static bool HasConnectedPins(const UEdGraphNode* InNode);

	static bool IsCommentNode(const UEdGraphNode* InNode);

	static bool IsPinUsesDefaultValue(const UEdGraphPin* InPin);

	static int32 GetCharNum(const FString& InString, const TCHAR& InChar);

	const static FSlateBrush* GetPlayIcon();

	const static FSlateBrush* GetStopIcon();

	static UAutoCommentGeneratorSettings* GetSettingsChecked();
};