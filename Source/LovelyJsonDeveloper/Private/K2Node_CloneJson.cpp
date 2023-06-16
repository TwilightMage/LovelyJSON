// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_CloneJson.h"

#include "EdGraphUtilities.h"
#include "KismetCompiledFunctionContext.h"
#include "KismetCompilerMisc.h"
#include "LovelyJsonFuncs.h"
#include "LovelyJsonUtils.h"

class Functor_CloneJson : public FNodeHandlingFunctor
{
public:
	Functor_CloneJson(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{

	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		UK2Node_CloneJson* TargetNode = CastChecked<UK2Node_CloneJson>(Node);
		UEdGraphPin* OutputPin = TargetNode->GetOutputPin();

		FNodeHandlingFunctor::RegisterNets(Context, Node);

		// Create a local term to drop the container into
		FBPTerminal* Term = Context.CreateLocalTerminalFromPinAutoChooseScope(OutputPin, Context.NetNameMap->MakeValidName(OutputPin));
		Term->Source = Node;
		Context.NetMap.Add(OutputPin, Term);
	}
	
	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		// Pick pins
		UK2Node_CloneJson* OwnerNode = CastChecked<UK2Node_CloneJson>(Node);
		UEdGraphPin* InputPin = OwnerNode->GetInputPin();
		UEdGraphPin* OutputPin = OwnerNode->GetOutputPin();

		// Find input value target for input pin
		FBPTerminal* InputTerm = nullptr;
		if (FBPTerminal** FoundTerm = Context.NetMap.Find(FEdGraphUtilities::GetNetFromPin(InputPin)))
		{
			InputTerm = *FoundTerm;
		}
		check(InputTerm);

		// Find output value target for output pin
		FBPTerminal* OutputTerm = nullptr;
		if (FBPTerminal** FoundTerm = Context.NetMap.Find(OutputPin))
		{
			OutputTerm = *FoundTerm;
		}
		check(OutputTerm);

		// Find function of corresponding type
		UFunction* Func = ULovelyJsonFuncs::StaticClass()->ClassDefaultObject->FindFunction("CloneJson");
		check(Func);

		// Create and setup statement
		FBlueprintCompiledStatement& JsonConvertStatement = Context.AppendStatementForNode(Node);
		JsonConvertStatement.Type = KCST_CallFunction;
		JsonConvertStatement.FunctionToCall = Func;
		JsonConvertStatement.RHS = {InputTerm};
		JsonConvertStatement.LHS = OutputTerm;
	}
};

FText UK2Node_CloneJson::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("CloneJson", "CloneJson", "Clone JSON");
}

void UK2Node_CloneJson::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, ULovelyJsonUtils::PC_JsonPinCategory, FNameParameterHelper(NAME_None), nullptr, FNameParameterHelper(FString("Source")), EPinContainerType::None, false);
	CreatePin(EGPD_Output, ULovelyJsonUtils::PC_JsonPinCategory, FNameParameterHelper(NAME_None), nullptr, FNameParameterHelper(FString("Copy")), EPinContainerType::None, true);
}

FNodeHandlingFunctor* UK2Node_CloneJson::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new Functor_CloneJson(CompilerContext);
}

UEdGraphPin* UK2Node_CloneJson::GetInputPin () const
{
	return FindPin(FName("Source"));
}

UEdGraphPin* UK2Node_CloneJson::GetOutputPin () const
{
	return FindPin(FName("Copy"));
}
