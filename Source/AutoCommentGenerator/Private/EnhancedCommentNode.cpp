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
			FString Comment = TEXT("Generating Comment");

			if (FAutoCommentGeneratorUtility::GetCharNum(CurrentComment, '.') < 3)
			{
				int32 DesiredDotNum = (FAutoCommentGeneratorUtility::GetCharNum(CurrentComment, '.') % 3) + 1;

				for (int32 i = 0; i < DesiredDotNum; i++)
				{
					Comment += TEXT(".");
				};
			}

			SetComment(Comment);

			AnimationElapsedSeconds = 0.f;
		}
	}
	else
	{
		AnimationElapsedSeconds = 0.f;

		if (GetNodeComment() != CurrentComment)
		{
			SetComment(CurrentComment);
		}
	}

	// button creation
	if (!bHasCreatedButton)
	{
		FVector2D TitleBarSize;

		if (TryGetTitleBarSize(TitleBarSize))
		{
			CreateButton(TitleBarSize);

			bHasCreatedButton = true;
		}
	}

	// button top padding control
	{
		FVector2D CurrentTitleBarSize;

		if (TryGetTitleBarSize(CurrentTitleBarSize))
		{
			if (PreviousTitleBarHeight != CurrentTitleBarSize.Y)
			{
				OnChangedTitleBarHeight(CurrentTitleBarSize.Y);

				PreviousTitleBarHeight = CurrentTitleBarSize.Y;
			}
		}
	}
}

void SEnhancedCommentNode::SetComment(const FString& NewComment)
{
	//TODO:enable Japanese input too

	OnCommentTextCommitted(FText::FromString(NewComment), ETextCommit::Type::Default);

	CurrentComment = NewComment;
}

TArray<UEdGraphNode*> SEnhancedCommentNode::GetNodesUnderThisComment()
{
	UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(GetNodeObj());

	if (!IsValid(CommentNode))
	{
		FAutoCommentGeneratorLogUtility::LogError(TEXT("Type of the node being observed by SEnhancedCommentNode is not UEdGraphNode_Comment."));
		
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
 	const TArray<UEdGraphNode*> ActiveNodesUnderThisComment = FAutoCommentGeneratorUtility::GetActiveNodes(GetNodesUnderThisComment());
 
 	if (ActiveNodesUnderThisComment.Num() == 0) return FString();

	const TArray<FNodeData> NodesData = FAutoCommentGeneratorUtility::GetNodesData(ActiveNodesUnderThisComment);

	FString JsonString;

	if (!FJsonObjectConverter::UStructToJsonObjectString(FNodesData(NodesData), JsonString, 0, 0))
	{
		FAutoCommentGeneratorLogUtility::LogError(TEXT("Failed to convert FNodesData to JSON string."));
	}

	return JsonString;
}

bool SEnhancedCommentNode::TryGetTitleBarSize(FVector2D& OutTitleBarSize) const
{
	OutTitleBarSize = GetDesiredSizeForMarquee();

	return !OutTitleBarSize.IsZero();
}

void SEnhancedCommentNode::SetButtonImage(const FSlateBrush* InSlateBrush)
{
	if (!ButtonImage.IsValid())
	{
		FAutoCommentGeneratorLogUtility::LogError(TEXT("Failed to set brush of the generate comment button."));

		return;
	}

	ButtonImage->SetImage(InSlateBrush);
}

void SEnhancedCommentNode::SetButtonTopPadding(float InButtonTopPadding)
{
	if (!ButtonBox.IsValid())
	{
		FAutoCommentGeneratorLogUtility::LogError(TEXT("Failed to set top padding of the generate comment button."));

		return;
	}

	ButtonBox->SetPadding(FMargin(0.f, InButtonTopPadding, 10.f, 0.f));
}

void SEnhancedCommentNode::CreateButton(const FVector2D& TitleBarSize)
{
	ButtonImage = SNew(SImage)
		.ColorAndOpacity(FLinearColor::White)
		.Image(FAutoCommentGeneratorUtility::GetPlayIcon());

	ButtonBox = SNew(SBox)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		.Padding(0.f, TitleBarSize.Y + 10.f, 10.f, 0.f)
		[
			SNew(SBox)
				.WidthOverride(15)
				.HeightOverride(15)
				[
					SNew(SButton)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.ContentPadding(0)
						.ButtonStyle(FAppStyle::Get(), TEXT("NoBorder"))
						.ButtonColorAndOpacity(GetCommentTitleBarColor())
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
				]
		];

	this->GetOrAddSlot(ENodeZone::TopRight)
		[
			ButtonBox.ToSharedRef()
		];
}

FReply SEnhancedCommentNode::OnClickedButton()
{
	if (bIsGeneratingComment)
	{
		StopGeneratingComment();
	}
	else
	{
		StartGeneratingComment();
	}

	return FReply::Handled();
}

void SEnhancedCommentNode::OnChangedTitleBarHeight(const float NewTitleBarHeight)
{
	SetButtonTopPadding(NewTitleBarHeight + 10.f);
}

void SEnhancedCommentNode::StartGeneratingComment()
{
	PreviousEnteredComment = GetNodeComment();

	bIsGeneratingComment = bIsPlayingAnimation = true;

	SetButtonImage(FAutoCommentGeneratorUtility::GetStopIcon());

	SetComment(TEXT("Generating Comment"));
}

void SEnhancedCommentNode::StopGeneratingComment()
{
	SetComment(PreviousEnteredComment);

	bIsGeneratingComment = bIsPlayingAnimation = false;

	SetButtonImage(FAutoCommentGeneratorUtility::GetPlayIcon());
}