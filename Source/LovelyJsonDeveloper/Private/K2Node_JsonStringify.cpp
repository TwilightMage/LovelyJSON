// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_JsonStringify.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "LovelyJsonFuncs.h"
#include "KismetCompiledFunctionContext.h"
#include "KismetCompilerMisc.h"
#include "LovelyJsonUtils.h"

class Functor_JsonStringify : public FNodeHandlingFunctor
{
public:
	Functor_JsonStringify(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		UK2Node_JsonStringify* TargetNode = CastChecked<UK2Node_JsonStringify>(Node);
		UEdGraphPin* OutputPin = TargetNode->GetOutputPin();

		FNodeHandlingFunctor::RegisterNets(Context, Node);

		// Create a local term to drop the container into
		GENERATE_BP_CREATE_OUT_TERM();
	}
	
	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		// Pick pins
		UK2Node_JsonStringify* OwnerNode = CastChecked<UK2Node_JsonStringify>(Node);
		UEdGraphPin* InputPin = OwnerNode->GetInputPin();
		UEdGraphPin* OutputPin = OwnerNode->GetOutputPin();

		GENERATE_BP_FIND_INPUT_TERM();
		GENERATE_BP_FIND_OUTPUT_TERM();

		// Find function of corresponding type
		UFunction* Func = ULovelyJsonFuncs::StaticClass()->ClassDefaultObject->FindFunction("JsonStringify");
		check(Func);

		// Create and setup statement
		GENERATE_BP_CREATE_FUNCTION_STATEMENT(JsonConvertStatement, GENERATE_BP_TERMS(InputTerm), OutputTerm, Func);
	}
};

FText UK2Node_JsonStringify::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_JsonStringify", "JsonStringify", "JSON Stringify");
}

void UK2Node_JsonStringify::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, JsonRefPinType, FName(FString("Value")));
	CreatePin(EGPD_Output, StringPinType, FName(FString("String")));
}

FNodeHandlingFunctor* UK2Node_JsonStringify::CreateNodeHandler (FKismetCompilerContext& CompilerContext) const
{
	return new Functor_JsonStringify(CompilerContext);
}

UEdGraphPin* UK2Node_JsonStringify::GetInputPin () const
{
	return FindPin(FName("Value"));
}

UEdGraphPin* UK2Node_JsonStringify::GetOutputPin() const
{
	return FindPin(FName("String"));
}
