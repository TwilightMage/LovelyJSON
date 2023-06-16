// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LovelyJsonUtils.generated.h"

UCLASS(Abstract)
class ULovelyJsonUtils : public UObject
{
	GENERATED_BODY()
public:
	ULovelyJsonUtils();

	inline static const FName PC_JsonPinCategory = "JSON";
	
	inline static UFunction* CreateObjectFunc;
	inline static TMap<EPinContainerType, TMap<FName, UFunction*>> FuncMap;
};

#define GENERATE_BP_TERMS(...) TArray<FBPTerminal*>{__VA_ARGS__}

#define GENERATE_BP_CREATE_TERM(Name, Pin)                                                                          \
FBPTerminal* Name = Context.CreateLocalTerminalFromPinAutoChooseScope(Pin, Context.NetNameMap->MakeValidName(Pin)); \
Name->Source = Node;                                                                                                \
Context.NetMap.Add(Pin, Name);                                                                                      \

#define GENERATE_BP_CREATE_OUT_TERM() GENERATE_BP_CREATE_TERM(OutputTerm, OutputPin)

#define GENERATE_BP_CREATE_LITERAL_TERM(VarName, LiteralName, Category, SubCategory, SubCategoryObject, ContainerType) \
FBPTerminal* VarName = Context.CreateLocalTerminal(TS_Literal);                                                        \
VarName->Source = Node;                                                                                                \
VarName->Name = (LiteralName);                                                                                         \
VarName->Type = FEdGraphPinType(                                                                                       \
	(Category),                                                                                                        \
	(SubCategory),                                                                                                     \
	(SubCategoryObject),                                                                                               \
	(ContainerType),                                                                                                   \
	false,                                                                                                             \
	FEdGraphTerminalType());                                                                                           \


#define GENERATE_BP_FIND_TERM(Name, Pin)                                                  \
FBPTerminal* Name = nullptr;                                                              \
if (FBPTerminal** FoundTerm = Context.NetMap.Find(FEdGraphUtilities::GetNetFromPin(Pin))) \
{                                                                                         \
	Name = *FoundTerm;                                                                    \
}                                                                                         \
check(Name);                                                                              \

#define GENERATE_BP_FIND_OUTPUT_TERM() GENERATE_BP_FIND_TERM(OutputTerm, OutputPin)
#define GENERATE_BP_FIND_INPUT_TERM() GENERATE_BP_FIND_TERM(InputTerm, InputPin)

#define GENERATE_BP_CREATE_FUNCTION_STATEMENT(Name, Inputs, Output, Func) \
FBlueprintCompiledStatement& Name = Context.AppendStatementForNode(Node); \
Name.Type = KCST_CallFunction;                                            \
Name.FunctionToCall = (Func);                                             \
Name.LHS = (Output);                                                      \
Name.RHS = (Inputs);                                                      \
