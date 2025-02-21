// Copyright (c) 2025, tsubasamusu All rights reserved.

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

FText SGptLanguageComboButton::GetDesiredComboButtonText() const
{
    if (SelectedCulture.IsValid()) return FText::FromString(SelectedCulture->GetNativeName());

    FCulturePtr CurrentCulture = FInternationalization::Get().GetCurrentLanguage();

    if (CurrentCulture.IsValid()) return FText::FromString(CurrentCulture->GetNativeName());

    return LOCTEXT("None", "None");
}

TSharedRef<SWidget> SGptLanguageComboButton::OnGetComboButtonMenuContent()
{
    FCulturePtr CurrentCulture = SelectedCulture.IsValid() ? SelectedCulture : FInternationalization::Get().GetCurrentLanguage();

    const auto& CulturePicker = SNew(SCulturePicker)
        .InitialSelection(CurrentCulture)
        .OnSelectionChanged_Lambda([this](const FCulturePtr& InSelectedCulture, ESelectInfo::Type SelectInfo)
            {
                SelectedCulture = InSelectedCulture;

                if (ComboButton.IsValid()) ComboButton->SetIsOpen(false);
            })
        .IsCulturePickable_Lambda([this](FCulturePtr Culture) -> bool
            {
                TArray<FString> CultureNames = Culture->GetPrioritizedParentCultureNames();

                for (const FString& CultureName : CultureNames)
                {
                    if (LocalizedCulturesFlyweight->LocalizedCultures.Contains(Culture)) return true;
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