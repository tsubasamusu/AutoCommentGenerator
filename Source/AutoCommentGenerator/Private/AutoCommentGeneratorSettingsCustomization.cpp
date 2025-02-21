// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorSettingsCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "GptLanguageComboButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "AutoCommentGeneratorSettingsCustomization"

TSharedRef<IDetailCustomization> FAutoCommentGeneratorSettingsCustomization::Create()
{
    return MakeShared<FAutoCommentGeneratorSettingsCustomization>();
}

void FAutoCommentGeneratorSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	ChangeApiKeyPropertyDisplayAsPassword(DetailLayout);

	AddGptLanguageProperty(DetailLayout);
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

void FAutoCommentGeneratorSettingsCustomization::AddGptLanguageProperty(IDetailLayoutBuilder& DetailLayout)
{
    const FName GptCategoryName = TEXT("GPT");
    const FText GptCategoryText = LOCTEXT("GptCategoryText", "GPT");
    const FText GptLanguagePropertyText = LOCTEXT("GptLanguagePropertyText", "GPT Language");

    IDetailCategoryBuilder& DetailCategoryBuilder = DetailLayout.EditCategory(GptCategoryName, GptCategoryText);

    const TSharedRef<FLocalizedCulturesFlyweight> LocalizedCulturesFlyweight = MakeShared<FLocalizedCulturesFlyweight>();

    DetailCategoryBuilder.AddCustomRow(GptLanguagePropertyText)
        .NameContent()
        [
            SNew(STextBlock)
                .Text(GptLanguagePropertyText)
                .Font(DetailLayout.GetDetailFont())
        ]
        .ValueContent()
        [
            SNew(SGptLanguageComboButton, LocalizedCulturesFlyweight)
        ];
}

#undef LOCTEXT_NAMESPACE