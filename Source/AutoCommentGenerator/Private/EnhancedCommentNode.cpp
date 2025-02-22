// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "EnhancedCommentNode.h"
#include "EdGraphNode_Comment.h"
#include "JsonObjectConverter.h"
#include "NodesData.h"
#include "AutoCommentGeneratorUtility.h"
#include "AutoCommentGeneratorLogUtility.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "CommentGenerator.h"
#include "AutoCommentGeneratorSettings.h"

void SEnhancedCommentNode::Construct(const FArguments& InArgs, UEdGraphNode_Comment* InNode)
{
	Super::Construct(SGraphNodeComment::FArguments(), InNode);

	CurrentComment = GetNodeComment();
}

void SEnhancedCommentNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// animation control
	if (bIsPlayingAnimation)
	{
		AnimationElapsedSeconds += InDeltaTime;

		if (AnimationElapsedSeconds >= AnimationSpan)
		{
			FString NewComment = GeneratingCommentText;

			if (FAutoCommentGeneratorUtility::GetCharNum(CurrentComment, '.') < 3)
			{
				int32 DesiredDotNum = (FAutoCommentGeneratorUtility::GetCharNum(CurrentComment, '.') % 3) + 1;

				for (int32 i = 0; i < DesiredDotNum; i++)
				{
					NewComment += TEXT(".");
				}
			}

			SetComment(NewComment);

			AnimationElapsedSeconds = 0.f;
		}
	}
	else
	{
		AnimationElapsedSeconds = 0.f;

		if (GetNodeComment() != CurrentComment) SetComment(GetNodeComment());
	}

	FVector2D CurrentTitleBarSize;

	if (!TryGetTitleBarSize(CurrentTitleBarSize)) return;

	// button creation
	if (!bHasCreatedButton)
	{
		CreateButton(CurrentTitleBarSize);

		bHasCreatedButton = true;
	}

	// button top padding control
	if (PreviousTitleBarHeight != CurrentTitleBarSize.Y)
	{
		OnChangedTitleBarHeight(CurrentTitleBarSize.Y);

		PreviousTitleBarHeight = CurrentTitleBarSize.Y;
	}

	// button appearance modification
	{
		UAutoCommentGeneratorSettings* AutoCommentGeneratorSettings = FAutoCommentGeneratorUtility::GetSettingsChecked();

		if (GetButtonSize() != AutoCommentGeneratorSettings->ButtonSize) SetButtonSize(AutoCommentGeneratorSettings->ButtonSize);

		if (GetButtonColor() != AutoCommentGeneratorSettings->ButtonColor) SetButtonColor(AutoCommentGeneratorSettings->ButtonColor);
	
		if (!ButtonPaddingMatchesSettings()) SetButtonPadding(CurrentTitleBarSize.Y + AutoCommentGeneratorSettings->ButtonTopPadding, AutoCommentGeneratorSettings->ButtonRightPadding);
	}
}

void SEnhancedCommentNode::SetComment(const FString& NewComment)
{
	OnCommentTextCommitted(FText::FromString(NewComment), ETextCommit::Type::Default);

	CurrentComment = NewComment;
}

TArray<UEdGraphNode*> SEnhancedCommentNode::GetNodesUnderThisComment()
{
	UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(GetNodeObj());

	check(IsValid(CommentNode));

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

bool SEnhancedCommentNode::TryGetNodesDataStringUnderThisComment(FString& OutNodesDataString)
{
	const TArray<UEdGraphNode*> ActiveNodesUnderThisComment = FAutoCommentGeneratorUtility::GetActiveNodes(GetNodesUnderThisComment());

	if (ActiveNodesUnderThisComment.Num() == 0) return false;

	const TArray<FNodeData> NodesData = FAutoCommentGeneratorUtility::GetNodesData(ActiveNodesUnderThisComment);

	return FJsonObjectConverter::UStructToJsonObjectString(FNodesData(NodesData), OutNodesDataString, 0, 0);
}

bool SEnhancedCommentNode::TryGetTitleBarSize(FVector2D& OutTitleBarSize) const
{
	OutTitleBarSize = GetDesiredSizeForMarquee();

	return !OutTitleBarSize.IsZero();
}

void SEnhancedCommentNode::SetButtonImage(const FSlateBrush* InSlateBrush)
{
	if (!ButtonImage.IsValid()) return;

	ButtonImage->SetImage(InSlateBrush);
}

void SEnhancedCommentNode::SetButtonPadding(const float InButtonTopPadding, const float InButtonRightPadding)
{
	if (!ButtonBoxForPadding.IsValid()) return;

	CurrentButtonPadding = FMargin(0.f, InButtonTopPadding, InButtonRightPadding, 0.f);

	ButtonBoxForPadding->SetPadding(CurrentButtonPadding);
}

void SEnhancedCommentNode::CreateButton(const FVector2D& TitleBarSize)
{
	UAutoCommentGeneratorSettings* AutoCommentGeneratorSettings = FAutoCommentGeneratorUtility::GetSettingsChecked();

	ButtonImage = SNew(SImage)
		.ColorAndOpacity(AutoCommentGeneratorSettings->ButtonColor)
		.Image(FAutoCommentGeneratorUtility::GetPlayIcon());

	ButtonBoxForSize = SNew(SBox)
		.WidthOverride(AutoCommentGeneratorSettings->ButtonSize.X)
		.HeightOverride(AutoCommentGeneratorSettings->ButtonSize.Y)
		[
			SNew(SButton)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.ContentPadding(0)
				.ButtonStyle(FAppStyle::Get(), TEXT("NoBorder"))
				.OnClicked(this, &SEnhancedCommentNode::OnClickedButton)
				.ToolTipText(FText::FromString("Generate comment using AI"))
				[
					SNew(SBox)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							ButtonImage.ToSharedRef()
						]
				]
		];

	ButtonBoxForPadding = SNew(SBox)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		[
			ButtonBoxForSize.ToSharedRef()
		];

	this->GetOrAddSlot(ENodeZone::TopRight)
		[
			ButtonBoxForPadding.ToSharedRef()
		];

	SetButtonPadding(TitleBarSize.Y + AutoCommentGeneratorSettings->ButtonTopPadding, AutoCommentGeneratorSettings->ButtonRightPadding);
}

FReply SEnhancedCommentNode::OnClickedButton()
{
	if (bIsGeneratingComment) StopGeneratingComment();

	else StartGeneratingComment();

	return FReply::Handled();
}

void SEnhancedCommentNode::OnChangedTitleBarHeight(const float NewTitleBarHeight)
{
	UAutoCommentGeneratorSettings* AutoCommentGeneratorSettings = FAutoCommentGeneratorUtility::GetSettingsChecked();

	SetButtonPadding(NewTitleBarHeight + AutoCommentGeneratorSettings->ButtonTopPadding, AutoCommentGeneratorSettings->ButtonRightPadding);
}

void SEnhancedCommentNode::StartGeneratingComment()
{
	PreviousEnteredComment = GetNodeComment();

	bIsGeneratingComment = bIsPlayingAnimation = true;

	SetButtonImage(FAutoCommentGeneratorUtility::GetStopIcon());

	SetComment(GeneratingCommentText);

	FString NodesDataString;

	if (!TryGetNodesDataStringUnderThisComment(NodesDataString))
	{
		SetComment(TEXT("This comment node does not contain active nodes."));

		StopGeneratingComment();

		return;
	}

	FCommentGenerator::GenerateComment(NodesDataString, [this](const bool bSucceeded, const FString& Message)
		{
			if (!bIsGeneratingComment) return;

			SetComment(Message);

			if (!bSucceeded) FAutoCommentGeneratorLogUtility::LogError(Message);

			StopGeneratingComment();
		});
}

void SEnhancedCommentNode::StopGeneratingComment()
{
	if (IsSetCommentForGenerating()) SetComment(PreviousEnteredComment);

	bIsGeneratingComment = bIsPlayingAnimation = false;

	SetButtonImage(FAutoCommentGeneratorUtility::GetPlayIcon());
}

bool SEnhancedCommentNode::IsSetCommentForGenerating() const
{
	return CurrentComment.Contains(GeneratingCommentText);
}

FVector2D SEnhancedCommentNode::GetButtonSize() const
{
	return ButtonBoxForSize.IsValid() ? ButtonBoxForSize->GetCachedGeometry().GetLocalSize() : FVector2D();
}

void SEnhancedCommentNode::SetButtonSize(const FVector2D& NewButtonSize)
{
	if (ButtonBoxForSize.IsValid())
	{
		ButtonBoxForSize->SetWidthOverride(NewButtonSize.X);
		ButtonBoxForSize->SetHeightOverride(NewButtonSize.Y);
	}
}

FSlateColor SEnhancedCommentNode::GetButtonColor() const
{
	return ButtonImage.IsValid() ? ButtonImage->GetForegroundColor() : FSlateColor();
}

void SEnhancedCommentNode::SetButtonColor(const FSlateColor& NewButtonColor)
{
	if (ButtonImage.IsValid()) ButtonImage->SetColorAndOpacity(NewButtonColor);
}

bool SEnhancedCommentNode::ButtonPaddingMatchesSettings() const
{
	FVector2D TitleBarSize;

	if (!TryGetTitleBarSize(TitleBarSize)) return false;

	UAutoCommentGeneratorSettings* AutoCommentGeneratorSettings = FAutoCommentGeneratorUtility::GetSettingsChecked();

	if (CurrentButtonPadding.Top - TitleBarSize.Y != AutoCommentGeneratorSettings->ButtonTopPadding) return false;

	if (CurrentButtonPadding.Right != AutoCommentGeneratorSettings->ButtonRightPadding) return false;

	return true;
}