// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NodesData.generated.h"

USTRUCT()
struct FPinData
{
    GENERATED_BODY()

public:
    UPROPERTY();
    FString PinName;

    UPROPERTY()
    FString PinId;

    UPROPERTY()
    TArray<FString> IdsOfPinsConnectedToMyself;

    UPROPERTY()
    FString PinType;
};

USTRUCT()
struct FNodeData
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FPinData> Pins;

    UPROPERTY()
    FString Comment;
};

USTRUCT()
struct FNodesData
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FNodeData> Nodes;

    FNodesData() :Nodes(TArray<FNodeData>()) {}

    FNodesData(const TArray<FNodeData>& InNodesData) :Nodes(InNodesData) {}
};