// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_Json.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "LovelyJsonFuncs.h"

void UK2Node_Json::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	if (GetClass() == UK2Node_Json::StaticClass()) return;
	
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_Json::GetMenuCategory () const
{
	return NSLOCTEXT("UK2Node_Json", "CategoryName", "JSON");
}

FSlateIcon UK2Node_Json::GetIconAndTint (FLinearColor& OutColor) const
{
	return FSlateIcon("LovelyJsonStyle", "JsonIcon");
}

FLinearColor UK2Node_Json::GetNodeTitleColor () const
{
	return FLinearColor(FVector(0.23, 0.23, 0.23));
}


