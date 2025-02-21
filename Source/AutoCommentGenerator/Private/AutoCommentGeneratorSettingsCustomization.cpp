// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorSettingsCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SEditableTextBox.h"

TSharedRef<IDetailCustomization> FAutoCommentGeneratorSettingsCustomization::Create()
{
    return MakeShared<FAutoCommentGeneratorSettingsCustomization>();
}

void FAutoCommentGeneratorSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	ChangeApiKeyPropertyDisplayAsPassword(DetailLayout);
}

void FAutoCommentGeneratorSettingsCustomization::ChangeApiKeyPropertyDisplayAsPassword(IDetailLayoutBuilder& DetailLayout)
{
    const FName GptCategoryName = TEXT("GPT");
    const FName ApiKeyPropertyName = TEXT("ApiKey");

    IDetailCategoryBuilder& GptCategory = DetailLayout.EditCategory(GptCategoryName);

    check(!GptCategory.IsEmpty());

    TArray<TSharedRef<IPropertyHandle>> GptPropertyHandles;

    GptCategory.GetDefaultProperties(GptPropertyHandles);

    for (const TSharedRef<IPropertyHandle>& GptPropertyHandle : GptPropertyHandles)
    {
        if (GptPropertyHandle->GetProperty()->GetName() != ApiKeyPropertyName) continue;

        GptCategory.AddProperty(GptPropertyHandle)
            .CustomWidget()
            .NameContent()
            [
                GptPropertyHandle->CreatePropertyNameWidget()
            ]
            .ValueContent()
            .MinDesiredWidth(250)
            [
                SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    [
                        SNew(SEditableTextBox)
                            .Text_Lambda([GptPropertyHandle]()
                                {
                                    FString Value;

                                    GptPropertyHandle->GetValue(Value);

                                    return FText::FromString(Value);
                                })
                            .OnTextCommitted_Lambda([GptPropertyHandle](const FText& NewText, ETextCommit::Type)
                                {
                                    GptPropertyHandle->SetValue(NewText.ToString());
                                })
                            .IsPassword(true)
                    ]
            ];
    }
}