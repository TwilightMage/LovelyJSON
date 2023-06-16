#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FLovelyJsonDeveloperModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    FString GetPluginContentPath() const;
    
    static inline const TSharedPtr<FSlateStyleSet> StyleSetInstance = MakeShared<FSlateStyleSet>("LovelyJsonStyle");
};
