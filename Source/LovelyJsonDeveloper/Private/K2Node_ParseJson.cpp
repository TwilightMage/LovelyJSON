// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_ParseJson.h"

#include "KismetCompiledFunctionContext.h"
#include "KismetCompilerMisc.h"
#include "LovelyJsonFuncs.h"

class Functor_ParseJson : public FNodeHandlingFunctor
{
public:
	Functor_ParseJson(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		UK2Node_ParseJson* TargetNode = CastChecked<UK2Node_ParseJson>(Node);
		UEdGraphPin* OutputPin = TargetNode->GetOutputPin();

		FNodeHandlingFunctor::RegisterNets(Context, Node);

		// Create a local term to drop the container into
		GENERATE_BP_CREATE_OUT_TERM();
	}
	
	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		// Pick pins
		UK2Node_ParseJson* OwnerNode = CastChecked<UK2Node_ParseJson>(Node);
		UEdGraphPin* InputPin = OwnerNode->GetInputPin();
		UEdGraphPin* OutputPin = OwnerNode->GetOutputPin();

		GENERATE_BP_FIND_INPUT_TERM();
		GENERATE_BP_FIND_OUTPUT_TERM();

		// Find function of corresponding type
		UFunction* Func = ULovelyJsonFuncs::StaticClass()->ClassDefaultObject->FindFunction("ParseJson");
		check(Func);

		// Create and setup statement
		GENERATE_BP_CREATE_FUNCTION_STATEMENT(JsonConvertStatement, GENERATE_BP_TERMS(InputTerm), OutputTerm, Func);
	}
};

FText UK2Node_ParseJson::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_ParseJson", "ParseJson", "Parse JSON");
}

void UK2Node_ParseJson::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, StringPinType, FName("String"));
	CreatePin(EGPD_Output, JsonRefPinType, FName("Value"));
}

FNodeHandlingFunctor* UK2Node_ParseJson::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new Functor_ParseJson(CompilerContext);
}

UEdGraphPin* UK2Node_ParseJson::GetInputPin() const
{
	return FindPin(FName("String"));
}

UEdGraphPin* UK2Node_ParseJson::GetOutputPin() const
{
	return FindPin(FName("Value"));
}
