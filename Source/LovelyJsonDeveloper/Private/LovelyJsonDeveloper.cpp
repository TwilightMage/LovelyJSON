#include "LovelyJsonDeveloper.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FstarisDeveloperModule"

#define SLATE_IMAGE_BRUSH(ImagePath, ImageSize) new FSlateImageBrush(StyleSetInstance->RootToContentDir(TEXT(ImagePath), TEXT(".png")), FVector2D(ImageSize, ImageSize))

void FLovelyJsonDeveloperModule::StartupModule()
{
	StyleSetInstance->SetContentRoot(GetPluginContentPath());

	StyleSetInstance->Set("JsonIcon", SLATE_IMAGE_BRUSH("/Icons/icon_20", 20.0f));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSetInstance);
}

void FLovelyJsonDeveloperModule::ShutdownModule()
{
	
}

FString FLovelyJsonDeveloperModule::GetPluginContentPath() const
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("LovelyJSON");

	if (Plugin.IsValid())
	{
		return Plugin->GetContentDir();
	}

	return FString();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLovelyJsonDeveloperModule, LovelyJsonDeveloper)