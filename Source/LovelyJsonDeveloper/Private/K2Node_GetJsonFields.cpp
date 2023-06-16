// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_GetJsonFields.h"

#include "Kismet2/BlueprintEditorUtils.h"

FText UK2Node_GetJsonFields::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_GetJsonFields", "GetJsonFields", "Get JSON Fields");
}

void UK2Node_GetJsonFields::AllocateDefaultPins()
{
	auto JsonPin = CreatePin(EGPD_Input, JsonRefPinType, bObjectOrArray ? FName("Object") : FName("Array"));

	for (int32 i = 0; i < Fields.Num(); i++)
	{
		auto KeyPin = CreatePin(EGPD_Input, bObjectOrArray ? KeyPinType : IndexPinType, bObjectOrArray ? FName("Key " + FString::FromInt(i)) : FName("Index " + FString::FromInt(i)));
		auto ValuePin = CreatePin(EGPD_Output, WildcardPinType, FName("Value " + FString::FromInt(i)));
	}
}

void UK2Node_GetJsonFields::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("GETJSONFIELDS", NSLOCTEXT("UK2Node_GetJsonFields", "ContextHeader", "Get JSON Fields"));

		if (Context->Pin != NULL)
		{
			if (Context->Pin->Direction == EGPD_Input && Context->Pin->ParentPin == nullptr)
			{
				Section.AddMenuEntry(
					"RemovePin",
					NSLOCTEXT("UK2Node_GetJsonFields", "RemovePin", "Remove key/value pair"),
					NSLOCTEXT("UK2Node_GetJsonFields", "RemovePinTooltip", "Remove this pin and it'd corresponding key/value pin"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject(const_cast<UK2Node_GetJsonFields*>(this), &UK2Node_GetJsonFields::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
					)
				);
			}
		}
		else
		{
			Section.AddMenuEntry(
				"SwitchType",
				bObjectOrArray ? NSLOCTEXT("UK2Node_GetJsonFields", "SwitchToArray", "Switch To Array") : NSLOCTEXT("UK2Node_GetJsonFields", "SwitchToObject", "Switch To Object"),
				bObjectOrArray ? NSLOCTEXT("UK2Node_GetJsonFields", "SwitchToArrayTooltip", "Make this node accept integer indices instead of string keys") : NSLOCTEXT("UK2Node_GetJsonFields", "SwitchToObjectTooltip", "Make this node accept string keys instead of integer indices"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_GetJsonFields*>(this), &UK2Node_GetJsonFields::SwitchType)
				)
			);
			
			Section.AddMenuEntry(
				"AddKeyValuePair",
				NSLOCTEXT("UK2Node_GetJsonFields", "AddPin", "Add key/value pair"),
				NSLOCTEXT("UK2Node_GetJsonFields", "AddPinTooltip", "Add another pair of key/value pin"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_GetJsonFields*>(this), &UK2Node_GetJsonFields::InteractiveAddPin)
				)
			);
		}
	}
}

void UK2Node_GetJsonFields::InteractiveAddPin ()
{
	FScopedTransaction Transaction(NSLOCTEXT("UK2Node_GetJsonFields", "AddPin", "Add pin"));
	AddInputPin();
}

void UK2Node_GetJsonFields::AddInputPin()
{
	auto KeyPin = CreatePin(EGPD_Input, bObjectOrArray ? KeyPinType : IndexPinType, bObjectOrArray ? FName("Key " + FString::FromInt(Fields.Num())) : FName("Index " + FString::FromInt(Fields.Num())));
	auto ValuePin = CreatePin(EGPD_Output, WildcardPinType, FName("Value " + FString::FromInt(Fields.Num())));

	Fields.Add({bObjectOrArray ? "" : FString::FromInt(Fields.Num()), WildcardPinType});
}

void UK2Node_GetJsonFields::RemoveInputPin(UEdGraphPin* Pin)
{
	IK2Node_AddPinInterface::RemoveInputPin(Pin);
}

void UK2Node_GetJsonFields::SwitchType()
{
	bObjectOrArray = !bObjectOrArray;

	GetInputPin()->PinName = bObjectOrArray ? FName("Object") : FName("Array");

	auto FieldPins = GetFieldPins();
	for (int32 i = 0; i < GetFieldPins().Num(); i++)
	{
		FieldPins[i].Key->PinType = bObjectOrArray ? KeyPinType : IndexPinType;
		FieldPins[i].Key->PinName = bObjectOrArray ? FName("Key " + FString::FromInt(i)) : FName("Index " + FString::FromInt(i));
	}

	if (!GetBlueprint()->bBeingCompiled)
	{
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	}
}

UEdGraphPin* UK2Node_GetJsonFields::GetInputPin()
{
	return Pins[0];
}

TArray<TPair<UEdGraphPin*, UEdGraphPin*>> UK2Node_GetJsonFields::GetFieldPins()
{
	TArray<TPair<UEdGraphPin*, UEdGraphPin*>> Pairs;
	TPair<UEdGraphPin*, UEdGraphPin*> Pair;
	for (int32 i = 1; i < Pins.Num(); i++)
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

	return Pairs;
}
