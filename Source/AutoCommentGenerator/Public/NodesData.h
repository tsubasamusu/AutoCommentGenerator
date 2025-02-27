// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NodesData.generated.h"

USTRUCT()
struct AUTOCOMMENTGENERATOR_API FPinData
{
    GENERATED_BODY()

public:
    UPROPERTY();
    FString PinName;

    UPROPERTY()
    FString PinType;

    UPROPERTY()
    FString PinId;

    UPROPERTY()
    FString DefaultValue;

    UPROPERTY()
    bool bDefaultValueIsUsed;

    UPROPERTY()
    TArray<FString> ConnectedPinIds;
};

USTRUCT()
struct AUTOCOMMENTGENERATOR_API FNodeData
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FString NodeName;

    UPROPERTY()
    FString Comment;

    UPROPERTY()
    bool bIsCommentNode;

    UPROPERTY()
    TArray<FPinData> Pins;
};

USTRUCT()
struct AUTOCOMMENTGENERATOR_API FNodesData
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FNodeData> Nodes;

    FNodesData() :Nodes(TArray<FNodeData>()) {}

    explicit FNodesData(const TArray<FNodeData>& InNodesData) :Nodes(InNodesData) {}
};