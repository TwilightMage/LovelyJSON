// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_GetJsonValueType.h"

#include "LovelyJsonFuncs.h"
#include "LovelyJsonUtils.h"

FText UK2Node_GetJsonValueType::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("GetJsonValueType", "GetJsonValueType", "Get JSON Value Type");
}

void UK2Node_GetJsonValueType::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, ULovelyJsonUtils::PC_JsonPinCategory, FNameParameterHelper(NAME_None), nullptr, false, true, FNameParameterHelper(FString("Value")));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Byte, FNameParameterHelper(NAME_None), StaticEnum<EJsonValueType>(), false, false, FNameParameterHelper(FString("Type")));
}

FNodeHandlingFunctor* UK2Node_GetJsonValueType::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return Super::CreateNodeHandler(CompilerContext);
}

UEdGraphPin* UK2Node_GetJsonValueType::GetInputPin() const
{
	return FindPin(FName("Value"));
}

UEdGraphPin* UK2Node_GetJsonValueType::GetOutputPin() const
{
	return FindPin(FName("Type"));
}
