// Copyright (c) 2025, tsubasamusu All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SComboButton;
class SWidget;

struct FLocalizedCulturesFlyweight
{
    TArray<FCultureRef> LocalizedCultures;

    FLocalizedCulturesFlyweight()
    {
        constexpr bool bIncludeDerivedCultures = false;

        const TArray<FString> LocalizedCultureNames = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Editor);

        LocalizedCultures = FInternationalization::Get().GetAvailableCultures(LocalizedCultureNames, bIncludeDerivedCultures);
    }
};

class SGptLanguageComboButton : public SCompoundWidget
{
    SLATE_BEGIN_ARGS(SGptLanguageComboButton) {}
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs, const TSharedRef<FLocalizedCulturesFlyweight>& InLocalizedCulturesFlyweight);

private:
    TSharedPtr<FLocalizedCulturesFlyweight> LocalizedCulturesFlyweight;

    TSharedPtr<SComboButton> ComboButton;

    FText GetDesiredComboButtonText() const;

    TSharedRef<SWidget> OnGetComboButtonMenuContent();
};