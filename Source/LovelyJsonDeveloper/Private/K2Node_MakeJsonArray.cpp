// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_MakeJsonArray.h"

#include "LovelyJsonUtils.h"
#include "Kismet2/BlueprintEditorUtils.h"

FText UK2Node_MakeJsonArray::GetNodeTitle (ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("MakeJsonArray", "MakeJsonArray", "Make JSON Array");
}

void UK2Node_MakeJsonArray::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, JsonRefPinType, FName(FString("Array")));
	
	for (int32 i = 0; i < NumInputs; i++)
	{
		CreatePin(EGPD_Input, WildcardPinType, FName("Item " + FString::FromInt(i)));
	}
}

void UK2Node_MakeJsonArray::AddInputPin()
{
	Modify();
	
	UEdGraphPin* PinValue = CreatePin(EGPD_Input, WildcardPinType, FName("Item " + FString::FromInt(NumInputs++)));

	const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
	if (!bIsCompiling)
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

void UK2Node_MakeJsonArray::RemoveInputPin(UEdGraphPin* Pin)
{
	IK2Node_AddPinInterface::RemoveInputPin(Pin);
}
