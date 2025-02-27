// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNodeComment.h"

class SImage;
class SBox;

class AUTOCOMMENTGENERATOR_API SEnhancedCommentNode final : public SGraphNodeComment
{
	typedef SGraphNodeComment Super;

public:
	SLATE_BEGIN_ARGS(SEnhancedCommentNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	void SetComment(const FString& NewComment);

	TArray<UEdGraphNode*> GetNodesUnderThisComment() const;

	bool TryGetNodesDataStringUnderThisComment(FString& OutNodesDataString) const;

	bool TryGetTitleBarSize(FVector2D& OutTitleBarSize) const;

	void SetButtonImage(const FSlateBrush* InSlateBrush) const;

	void SetButtonPadding(const float InButtonTopPadding, const float InButtonRightPadding);

	void CreateButton(const FVector2D& TitleBarSize);

	FReply OnClickedButton();

	void OnChangedTitleBarHeight(const float NewTitleBarHeight);

	void StartGeneratingComment();

	void StopGeneratingComment();

	bool IsSetCommentForGenerating() const;

	FVector2D GetButtonSize() const;

	void SetButtonSize(const FVector2D& NewButtonSize) const;

	FSlateColor GetButtonColor() const;

	void SetButtonColor(const FSlateColor& NewButtonColor) const;

	bool ButtonPaddingMatchesSettings() const;

	bool bHasCreatedButton = false;

	bool bIsGeneratingComment = false;

	bool bIsPlayingAnimation = false;

	float PreviousTitleBarHeight = 0;

	float AnimationElapsedSeconds = 0;

	const float AnimationSpan = 0.3f;

	FString PreviousEnteredComment;

	FString CurrentComment;

	const FString GeneratingCommentText = TEXT("Generating Comment");

	FMargin CurrentButtonPadding;

	TSharedPtr<SImage> ButtonImage;

	TSharedPtr<SBox> ButtonBoxForPadding;

	TSharedPtr<SBox> ButtonBoxForSize;
};