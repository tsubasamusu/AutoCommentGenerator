// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNodeComment.h"

class SImage;
class SBox;

class SEnhancedCommentNode final : public SGraphNodeComment
{
	typedef SGraphNodeComment Super;

public:
	SLATE_BEGIN_ARGS(SEnhancedCommentNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	void SetComment(const FString& NewComment);

	TArray<UEdGraphNode*> GetNodesUnderThisComment();

	bool TryGetNodesDataStringUnderThisComment(FString& OutNodesDataString);

	bool TryGetTitleBarSize(FVector2D& OutTitleBarSize) const;

	void SetButtonImage(const FSlateBrush* InSlateBrush);

	void SetButtonTopPadding(const float InButtonTopPadding);

	void CreateButton(const FVector2D& TitleBarSize);

	FReply OnClickedButton();

	void OnChangedTitleBarHeight(const float NewTitleBarHeight);

	void StartGeneratingComment();

	void StopGeneratingComment();

	bool IsSetCommentForGenerating() const;

	FVector2D GetButtonSize() const;

	void SetButtonSize(const FVector2D& NewButtonSize);

	bool bHasCreatedButton;

	bool bIsGeneratingComment;

	bool bIsPlayingAnimation;

	float PreviousTitleBarHeight;

	float AnimationElapsedSeconds;

	const float AnimationSpan = 0.3f;

	FString PreviousEnteredComment;

	FString CurrentComment;

	const FString GeneratingCommentText = TEXT("Generating Comment");

	TSharedPtr<SImage> ButtonImage;

	TSharedPtr<SBox> ButtonBoxForPadding;

	TSharedPtr<SBox> ButtonBoxForSize;
};