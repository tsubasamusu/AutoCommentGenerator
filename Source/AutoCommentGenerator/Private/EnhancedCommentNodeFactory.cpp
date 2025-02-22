// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "EnhancedCommentNodeFactory.h"
#include "EdGraphNode_Comment.h"
#include "EnhancedCommentNode.h"

TSharedPtr<SGraphNode> FEnhancedCommentNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
    UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(Node);

    if (IsValid(CommentNode))
    {
        return SNew(SEnhancedCommentNode, CommentNode);
    }

    return nullptr;
}