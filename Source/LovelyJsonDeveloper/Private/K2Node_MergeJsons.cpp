// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_MergeJsons.h"

#include "LovelyJsonUtils.h"

FText UK2Node_MergeJsons::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_MergeJsons", "MergeJsons", "Merge JSONs");
}

void UK2Node_MergeJsons::AllocateDefaultPins()
{
	UEdGraphPin* SourcePin = CreatePin(EGPD_Input, JsonRefPinType, FName(FString("Source")));
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, JsonRefPinType, FName(FString("Result")));

	for (int32 i = 0; i < NumMerges; i++)
	{
		UEdGraphPin* MergePin = CreatePin(EGPD_Input, JsonValPinType, FName("Merge " + FString::FromInt(i + 1)));
	}
}

FNodeHandlingFunctor* UK2Node_MergeJsons::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return Super::CreateNodeHandler(CompilerContext);
}

void UK2Node_MergeJsons::AddInputPin()
{
	UEdGraphPin* MergePin = CreatePin(EGPD_Input, JsonValPinType, FName("Merge " + FString::FromInt(NumMerges++)));
}
