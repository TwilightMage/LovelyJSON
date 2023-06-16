// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_MakeJsonObject.h"

#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "KismetCompiledFunctionContext.h"
#include "KismetCompilerMisc.h"
#include "LovelyJsonUtils.h"
#include "ScopedTransaction.h"
#include "Kismet2/BlueprintEditorUtils.h"

class Functor_MakeJsonObject : public FNodeHandlingFunctor
{
public:
	Functor_MakeJsonObject(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		UK2Node_MakeJsonObject* TargetNode = CastChecked<UK2Node_MakeJsonObject>(Node);
		TArray<TPair<UEdGraphPin*, UEdGraphPin*>> InputPins = TargetNode->GetInputPins();
		UEdGraphPin* OutputPin = TargetNode->GetOutputPin();

		FNodeHandlingFunctor::RegisterNets(Context, Node);

		GENERATE_BP_CREATE_OUT_TERM()
		
		for (int32 i = 0; i < InputPins.Num(); i++)
		{
			if (InputPins[i].Value->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
			{
				// Local container to store struct class
				GENERATE_BP_CREATE_LITERAL_TERM(StructClassTerm, FString::Printf(TEXT("Class_%i"), i), UEdGraphSchema_K2::PC_Object, NAME_None, UScriptStruct::StaticClass(), EPinContainerType::None);
				StructClassTerm->ObjectLiteral = InputPins[i].Value->PinType.PinSubCategoryObject.Get();
			}
		}
	}
	
	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		// Pick pins
		UK2Node_MakeJsonObject* OwnerNode = CastChecked<UK2Node_MakeJsonObject>(Node);
		TArray<TPair<UEdGraphPin*, UEdGraphPin*>> InputPins = OwnerNode->GetInputPins();
		UEdGraphPin* OutputPin = OwnerNode->GetOutputPin();

		TMap<UEdGraphPin*, FBPTerminal*> ClassLiterals;
		for (auto& Literal : Context.Literals)
		{
			if (Literal.Source == OwnerNode)
			{
				FString LitName;
				FString LitIndex;
				Literal.Name.Split(TEXT("_"), &LitName, &LitIndex);

				if (LitName != "Class") continue;

				const int32 Index = FCString::Atoi(*LitIndex);
				
				ClassLiterals.Add(InputPins[Index].Value, &Literal);
			}
		}

		// Find output value target for output pin
		GENERATE_BP_FIND_OUTPUT_TERM();

		// Create Basic JSON object setup
		GENERATE_BP_CREATE_FUNCTION_STATEMENT(JsonCreationStatement, GENERATE_BP_TERMS(), OutputTerm, ULovelyJsonUtils::CreateObjectFunc);

		// Assign values to json
		for (const auto& PinPair : InputPins)
		{
			GENERATE_BP_FIND_TERM(InputKeyTerm, PinPair.Key);
			GENERATE_BP_FIND_TERM(InputValueTerm, PinPair.Value);

			if (const auto Func = ULovelyJsonUtils::FuncMap[PinPair.Value->PinType.ContainerType][PinPair.Value->PinType.PinCategory])
			{
				if (InputValueTerm->Type.PinCategory == UEdGraphSchema_K2::PC_Struct)
				{
					GENERATE_BP_FIND_TERM(ClassLiteral, PinPair.Value);
					GENERATE_BP_CREATE_FUNCTION_STATEMENT(JsonSetPropertyStatement, GENERATE_BP_TERMS(OutputTerm, InputKeyTerm, ClassLiteral, InputValueTerm), nullptr, Func);
				}
				else
				{
					GENERATE_BP_CREATE_FUNCTION_STATEMENT(JsonSetPropertyStatement, GENERATE_BP_TERMS(OutputTerm, InputKeyTerm, InputValueTerm), nullptr, Func);
				}
			}
			else
			{
				Context.MessageLog.Warning(TEXT("Corresponding JSON function for @@ was not compiled. This field will be skipped."), PinPair.Value);
			}
		}
	}
};

FText UK2Node_MakeJsonObject::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("MakeJsonObject", "MakeJsonObject", "Make JSON Object");
}

void UK2Node_MakeJsonObject::AllocateDefaultPins()
{
	auto OutPin = CreatePin(EGPD_Output, JsonRefPinType, FName(FString("Object")));
	
	for (int32 i = 0; i < Fields.Num(); i++)
	{
		UEdGraphPin* PinName = CreatePin(EGPD_Input, KeyPinType, FName("Key " + FString::FromInt(i)));
		UEdGraphPin* PinValue = CreatePin(EGPD_Input, WildcardPinType, FName("Value " + FString::FromInt(i)));

		if (!Fields[i].Name.IsEmpty())
		{
			PinName->DefaultValue = Fields[i].Name;
		}
		PinValue->PinType = Fields[i].Type;
	}
}

FNodeHandlingFunctor* UK2Node_MakeJsonObject::CreateNodeHandler(FKismetCompilerContext& CompilerContext) const
{
	return new Functor_MakeJsonObject(CompilerContext);
}

void UK2Node_MakeJsonObject::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Modify();

	SynchronizePins(Pin);
}

void UK2Node_MakeJsonObject::PinTypeChanged(UEdGraphPin* Pin)
{
	SynchronizePins(Pin);

	Super::PinTypeChanged(Pin);
}

bool UK2Node_MakeJsonObject::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin->Direction == EGPD_Input && MyPin->PinName.ToString().StartsWith("Value "))
	{
		const FName& OtherPinCategory = OtherPin->PinType.PinCategory;
		const EPinContainerType OtherPinContainer = OtherPin->PinType.ContainerType;

		static TSet<FName> SupportedTypes = {
			UEdGraphSchema_K2::PC_Text,
			UEdGraphSchema_K2::PC_String,
			UEdGraphSchema_K2::PC_Name,
			UEdGraphSchema_K2::PC_Boolean,
			UEdGraphSchema_K2::PC_Byte,
			UEdGraphSchema_K2::PC_Int,
			UEdGraphSchema_K2::PC_Int64,
			UEdGraphSchema_K2::PC_Float,
			UEdGraphSchema_K2::PC_Struct,
			UEdGraphSchema_K2::PC_SoftClass,
			UEdGraphSchema_K2::PC_SoftObject,
			ULovelyJsonUtils::PC_JsonPinCategory
		};

		if (!SupportedTypes.Contains(OtherPinCategory) || OtherPinCategory == UEdGraphSchema_K2::PC_Text && OtherPinContainer == EPinContainerType::Set)
		{
			OutReason = NSLOCTEXT("MakeJsonObject", "InvalidArgumentType", "This argument type is not supported.").ToString();
			return true;
		}

		if (OtherPinCategory == UEdGraphSchema_K2::PC_Struct)
		{
			OutReason = NSLOCTEXT("MakeJsonObject", "StructNotSupported", "Struct type support is not yet implemented.").ToString();
			return true;
		}

		if (OtherPinContainer == EPinContainerType::Map)
		{
			OutReason = NSLOCTEXT("MakeJsonObject", "MapNotSupported", "Map container support is not yet implemented.").ToString();
			return true;
		}

		return false;
	}

	return Super::IsConnectionDisallowed(MyPin, OtherPin, OutReason);
}

void UK2Node_MakeJsonObject::GetNodeContextMenuActions (UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("MAKEJSONOBJECT", NSLOCTEXT("MakeJsonObject", "ContextHeader", "Make JSON Object"));

		if (Context->Pin != NULL)
		{
			if (Context->Pin->Direction == EGPD_Input && Context->Pin->ParentPin == nullptr)
			{
				Section.AddMenuEntry(
					"RemovePin",
					NSLOCTEXT("MakeJsonObject", "RemovePin", "Remove key/value pair"),
					NSLOCTEXT("MakeJsonObject", "RemovePinTooltip", "Remove this pin and it'd corresponding key/value pin"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject(const_cast<UK2Node_MakeJsonObject*>(this), &UK2Node_MakeJsonObject::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
					)
				);
			}
		}
		else
		{
			Section.AddMenuEntry(
				"AddKeyValuePair",
				NSLOCTEXT("MakeJsonObject", "AddPin", "Add key/value pair"),
				NSLOCTEXT("MakeJsonObject", "AddPinTooltip", "Add another pair of key/value pin"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_MakeJsonObject*>(this), &UK2Node_MakeJsonObject::InteractiveAddPin)
				)
			);
		}
	}
}

void UK2Node_MakeJsonObject::InteractiveAddPin()
{
	FScopedTransaction Transaction(NSLOCTEXT("MakeJsonObject", "AddPin", "Add pin"));
	AddInputPin();
}

void UK2Node_MakeJsonObject::AddInputPin()
{
	Modify();
	
	UEdGraphPin* PinName = CreatePin(EGPD_Input, KeyPinType, FName("Key " + FString::FromInt(Fields.Num())));
	UEdGraphPin* PinValue = CreatePin(EGPD_Input, WildcardPinType, FName("Value " + FString::FromInt(Fields.Num())));

	Fields.Add({"", WildcardPinType});
	
	const bool bIsCompiling = GetBlueprint()->bBeingCompiled;
	if (!bIsCompiling)
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

void UK2Node_MakeJsonObject::RemoveInputPin(UEdGraphPin* Pin)
{
	check(Pin->Direction == EGPD_Input);
	checkSlow(Pins.Contains(Pin));
	
	TArray<TPair<UEdGraphPin*, UEdGraphPin*>> InputPins = GetInputPins();
	for (int32 i = 0; i < InputPins.Num(); i++)
	{
		bool bFound = false;
		
		if (InputPins[i].Key == Pin)
		{
			bFound = true;
		}
		else if (InputPins[i].Value == Pin)
		{
			bFound = true;
		}

		if (bFound)
		{
			FScopedTransaction Transaction(NSLOCTEXT("MakeJsonObject", "RemovePin", "Remove pin"));
			Modify();

			Pins.Remove(InputPins[i].Key);
			Pins.Remove(InputPins[i].Value);
			
			InputPins[i].Key->MarkAsGarbage();
			InputPins[i].Value->MarkAsGarbage();

			Fields.RemoveAt(i);

			PinConnectionListChanged(nullptr);

			return;
		}
	}
}

TArray<TPair<UEdGraphPin*, UEdGraphPin*>> UK2Node_MakeJsonObject::GetInputPins()
{
	TArray<TPair<UEdGraphPin*, UEdGraphPin*>> Pairs;
	TPair<UEdGraphPin*, UEdGraphPin*> Pair;
	for (int32 i = 0; i < Pins.Num(); i++)
	{
		if (Pins[i]->Direction == EGPD_Input)
		{
			if (!Pair.Key)
			{
				Pair.Key = Pins[i];
			}
			else
			{
				Pair.Value = Pins[i];
				Pairs.Add(Pair);
				
				Pair.Key = nullptr;
				Pair.Value = nullptr;
			}
		}
	}

	return Pairs;
}

UEdGraphPin* UK2Node_MakeJsonObject::GetOutputPin()
{
	return FindPin(FName("Object"));
}

void UK2Node_MakeJsonObject::SynchronizePins(UEdGraphPin* InstigatorPin)
{
	bool bModified = false;
	
	if (InstigatorPin && InstigatorPin->Direction == EGPD_Input)
	{
		if (InstigatorPin->LinkedTo.IsEmpty())
		{
			if (!InstigatorPin->GetName().StartsWith("Key "))
			{
				if (InstigatorPin->PinType != WildcardPinType)
				{
					InstigatorPin->PinType = WildcardPinType;
					bModified = true;
				}
			}
		}
		else
		{
			UEdGraphPin* ArgumentSourcePin = InstigatorPin->LinkedTo[0];

			if (InstigatorPin->PinType != ArgumentSourcePin->PinType)
			{
				InstigatorPin->PinType = ArgumentSourcePin->PinType;
				bModified = true;

				InstigatorPin->PinType.bIsReference = InstigatorPin->PinType.PinCategory == ULovelyJsonUtils::PC_JsonPinCategory;
			}
		}
	}

	const TArray<TPair<UEdGraphPin*, UEdGraphPin*>> InputPins = GetInputPins();
	for (int32 i = 0; i < FMath::Min(InputPins.Num(), Fields.Num()); i++)
	{
		FName KeyName = FName("Key " + FString::FromInt(i));
		FName ValueName = FName("Value " + FString::FromInt(i));

		// Sync input key name
		if (InputPins[i].Key->PinName != KeyName)
		{
			InputPins[i].Key->PinName = KeyName;
		
			bModified = true;
		}

		// Sync input value name
		if (InputPins[i].Value->PinName != ValueName)
		{
			InputPins[i].Value->PinName = ValueName;
			
			bModified = true;
		}

		// Sync input value type
		FEdGraphPinType RequiredType = InputPins[i].Value->LinkedTo.IsEmpty() ? WildcardPinType : InputPins[i].Value->LinkedTo[0]->PinType;
		if (InputPins[i].Value->PinType != RequiredType)
		{
			InputPins[i].Value->PinType = RequiredType;
			
			bModified = true;
		}

		// Sync fields to pins
		Fields[i].Name = InputPins[i].Key->LinkedTo.IsEmpty() ? InputPins[i].Key->DefaultValue : "";
		Fields[i].Type = RequiredType;
	}

	// Add missing pins
	for (int32 i = InputPins.Num(); i < Fields.Num(); i++)
	{
		UEdGraphPin* PinName = CreatePin(EGPD_Input, KeyPinType, FName("Key " + FString::FromInt(Fields.Num())));
		UEdGraphPin* PinValue = CreatePin(EGPD_Input, WildcardPinType, FName("Value " + FString::FromInt(Fields.Num())));
		
		if (!GetBlueprint()->bBeingCompiled)
		{
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
		}
	}

	// Add missing fields
	for (int32 i = Fields.Num(); i < InputPins.Num(); i++)
	{
		FJsonFieldMetadata Field;
		Field.Name = InputPins[i].Key->LinkedTo.IsEmpty() ? InputPins[i].Key->DefaultValue : "";
		Field.Type = InputPins[i].Value->PinType;

		Fields.Add(Field);
	}

	if (bModified)
	{
		GetGraph()->NotifyGraphChanged();

		UBlueprint* Blueprint = GetBlueprint();
		if (!Blueprint->bBeingCompiled)
		{
			FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
			Blueprint->BroadcastChanged();
		}
	}
}
