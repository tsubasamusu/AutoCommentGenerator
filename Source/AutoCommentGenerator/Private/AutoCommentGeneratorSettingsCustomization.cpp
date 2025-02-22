// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorSettingsCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EditorStyleSet.h"
#include "GptLanguageComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "IDocumentation.h"

#define LOCTEXT_NAMESPACE "AutoCommentGeneratorSettingsCustomization"

TSharedRef<IDetailCustomization> FAutoCommentGeneratorSettingsCustomization::Create()
{
    return MakeShared<FAutoCommentGeneratorSettingsCustomization>();
}

void FAutoCommentGeneratorSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	ChangeApiKeyPropertyDisplayAsPassword(DetailLayout);

	AddGptLanguageProperty(DetailLayout);

    AddGptModelsDocumentButton(DetailLayout);
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

void FAutoCommentGeneratorSettingsCustomization::AddGptModelsDocumentButton(IDetailLayoutBuilder& DetailLayout)
{
    const FName GptCategoryName = TEXT("GPT");
    const FName GptModelPropertyName = TEXT("GptModelName");

    IDetailCategoryBuilder& GptCategory = DetailLayout.EditCategory(GptCategoryName);

    check(!GptCategory.IsEmpty());

    TArray<TSharedRef<IPropertyHandle>> GptPropertyHandles;

    GptCategory.GetDefaultProperties(GptPropertyHandles);

    for (const TSharedRef<IPropertyHandle>& GptPropertyHandle : GptPropertyHandles)
    {
        if (GptPropertyHandle->GetProperty()->GetName() != GptModelPropertyName) continue;

        GptCategory.AddProperty(GptPropertyHandle)
            .CustomWidget()
            .NameContent()
            [
                GptPropertyHandle->CreatePropertyNameWidget()
            ]
            .ValueContent()
            .MinDesiredWidth(200.f)
            [
                SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    [
                        SNew(SEditableTextBox)
                            .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                            .HintText(FText::FromString(TEXT("gpt-4")))
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
                    ]
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5.f, 0.f, 0.f, 0.f)
                    [
                        SNew(SButton)
                            .ButtonStyle(FAppStyle::Get(), TEXT("NoBorder"))
                            .ButtonColorAndOpacity(FLinearColor::White)
                            .ToolTipText(FText::FromString(TEXT("View the types of GPT models currently available")))
                            .OnClicked_Lambda([]()
                                {
                                    IDocumentation::Get()->Open(TEXT("https://platform.openai.com/docs/models"));

                                    return FReply::Handled();
                                })
                            [
                                SNew(SBox)
                                    .HAlign(HAlign_Fill)
                                    .VAlign(VAlign_Fill)
                                    [
                                        SNew(SImage)
                                            .ColorAndOpacity(FLinearColor::White)
                                            .Image(FAppStyle::Get().GetBrush(TEXT("MainFrame.VisitSearchForAnswersPage")))
                                    ]
                            ]
                    ]
            ];
    }
}

#undef LOCTEXT_NAMESPACE