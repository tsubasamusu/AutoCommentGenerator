// Copyright (c) 2025, tsubasamusu All rights reserved.

#include "AutoCommentGeneratorSettings.h"
#include "AutoCommentGeneratorUtility.h"
#include "GptLanguageComboButton.h"
#include "Internationalization/Culture.h"
#include "SCulturePicker.h"
#include "Widgets/Input/SComboButton.h"

#define LOCTEXT_NAMESPACE "GptLanguageComboButton"

void SGptLanguageComboButton::Construct(const FArguments& InArgs, const TSharedRef<FLocalizedCulturesFlyweight>& InLocalizedCulturesFlyweight)
{
    LocalizedCulturesFlyweight = InLocalizedCulturesFlyweight;

    ChildSlot
        [
            SAssignNew(ComboButton, SComboButton)
                .ButtonContent()
                [
                    SNew(STextBlock)
                        .Text(this, &SGptLanguageComboButton::GetDesiredComboButtonText)
                ]
        ];

    ComboButton->SetOnGetMenuContent(FOnGetContent::CreateSP(this, &SGptLanguageComboButton::OnGetComboButtonMenuContent));
}

// ReSharper disable once CppMemberFunctionMayBeStatic
FText SGptLanguageComboButton::GetDesiredComboButtonText() const
{
    return FText::FromString(FAutoCommentGeneratorUtility::GetSettingsChecked()->GetGptLanguageCulture()->GetNativeName());
}

TSharedRef<SWidget> SGptLanguageComboButton::OnGetComboButtonMenuContent() const
{
    const auto& CulturePicker = SNew(SCulturePicker)
        .InitialSelection(FAutoCommentGeneratorUtility::GetSettingsChecked()->GetGptLanguageCulture())
        .OnSelectionChanged_Lambda([this](const FCulturePtr& InSelectedCulture, ESelectInfo::Type SelectInfo)
            {
                FAutoCommentGeneratorUtility::GetSettingsChecked()->SetGptLanguageCulture(InSelectedCulture);

                if (ComboButton.IsValid())
                {
                    ComboButton->SetIsOpen(false);
                }
            })
        .IsCulturePickable_Lambda([this](const FCulturePtr& Culture) -> bool
            {
                for (TArray<FString> CultureNames = Culture->GetPrioritizedParentCultureNames(); const FString& CultureName : CultureNames)
                {
                    if (LocalizedCulturesFlyweight->LocalizedCultures.Contains(Culture))
                    {
                        return true;
                    }
                }

                return false;
            })
        .DisplayNameFormat(SCulturePicker::ECultureDisplayFormat::ActiveAndNativeCultureDisplayName)
        .ViewMode(SCulturePicker::ECulturesViewMode::Flat);

    return SNew(SBox)
        .MaxDesiredHeight(500.0f)
        .WidthOverride(350.0f)
        [
            CulturePicker
        ];
}

#undef LOCTEXT_NAMESPACE