// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SetJsonFields.h"

#include "Kismet2/BlueprintEditorUtils.h"

FText UK2Node_SetJsonFields::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("UK2Node_SetJsonFields", "SetJsonFields", "Set JSON Fields");
}

void UK2Node_SetJsonFields::AllocateDefaultPins()
{
	auto JsonPin = CreatePin(EGPD_Input, JsonRefPinType, bObjectOrArray ? FName("Object") : FName("Array"));
	auto ResultPin = CreatePin(EGPD_Output, JsonRefPinType, FName("Result"));

	for (int32 i = 0; i < Fields.Num(); i++)
	{
		auto KeyPin = CreatePin(EGPD_Input, bObjectOrArray ? KeyPinType : IndexPinType, bObjectOrArray ? FName("Key " + FString::FromInt(i)) : FName("Index " + FString::FromInt(i)));
		auto ValuePin = CreatePin(EGPD_Input, WildcardPinType, FName("Value " + FString::FromInt(i)));
	}
}

void UK2Node_SetJsonFields::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection("SETJSONFIELDS", NSLOCTEXT("UK2Node_SetJsonFields", "ContextHeader", "Set JSON Fields"));

		if (Context->Pin != NULL)
		{
			if (Context->Pin->Direction == EGPD_Input && Context->Pin->ParentPin == nullptr)
			{
				Section.AddMenuEntry(
					"RemovePin",
					NSLOCTEXT("UK2Node_SetJsonFields", "RemovePin", "Remove key/value pair"),
					NSLOCTEXT("UK2Node_SetJsonFields", "RemovePinTooltip", "Remove this pin and it'd corresponding key/value pin"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject(const_cast<UK2Node_SetJsonFields*>(this), &UK2Node_SetJsonFields::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
					)
				);
			}
		}
		else
		{
			Section.AddMenuEntry(
				"SwitchType",
				bObjectOrArray ? NSLOCTEXT("UK2Node_SetJsonFields", "SwitchToArray", "Switch To Array") : NSLOCTEXT("UK2Node_SetJsonFields", "SwitchToObject", "Switch To Object"),
				bObjectOrArray ? NSLOCTEXT("UK2Node_SetJsonFields", "SwitchToArrayTooltip", "Make this node accept integer indices instead of string keys") : NSLOCTEXT("UK2Node_SetJsonFields", "SwitchToObjectTooltip", "Make this node accept string keys instead of integer indices"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_SetJsonFields*>(this), &UK2Node_SetJsonFields::SwitchType)
				)
			);
			
			Section.AddMenuEntry(
				"AddKeyValuePair",
				NSLOCTEXT("UK2Node_SetJsonFields", "AddPin", "Add key/value pair"),
				NSLOCTEXT("UK2Node_SetJsonFields", "AddPinTooltip", "Add another pair of key/value pin"),
				FSlateIcon(),
				FUIAction(
					FExecuteAction::CreateUObject(const_cast<UK2Node_SetJsonFields*>(this), &UK2Node_SetJsonFields::InteractiveAddPin)
				)
			);
		}
	}
}

void UK2Node_SetJsonFields::InteractiveAddPin ()
{
	FScopedTransaction Transaction(NSLOCTEXT("UK2Node_SetJsonFields", "AddPin", "Add pin"));
	AddInputPin();
}

void UK2Node_SetJsonFields::AddInputPin()
{
	auto KeyPin = CreatePin(EGPD_Input, bObjectOrArray ? KeyPinType : IndexPinType, bObjectOrArray ? FName("Key " + FString::FromInt(Fields.Num())) : FName("Index " + FString::FromInt(Fields.Num())));
	auto ValuePin = CreatePin(EGPD_Input, WildcardPinType, FName("Value " + FString::FromInt(Fields.Num())));

	Fields.Add({bObjectOrArray ? "" : FString::FromInt(Fields.Num()), WildcardPinType});
}

void UK2Node_SetJsonFields::RemoveInputPin(UEdGraphPin* Pin)
{
	IK2Node_AddPinInterface::RemoveInputPin(Pin);
}

void UK2Node_SetJsonFields::SwitchType()
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

UEdGraphPin* UK2Node_SetJsonFields::GetInputPin()
{
	return Pins[0];
}

TArray<TPair<UEdGraphPin*, UEdGraphPin*>> UK2Node_SetJsonFields::GetFieldPins()
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