// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_Json.h"
#include "K2Node_SetJsonFields.generated.h"

/**
 * 
 */
UCLASS()
class LOVELYJSONDEVELOPER_API UK2Node_SetJsonFields : public UK2Node_Json, public IK2Node_AddPinInterface
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;

	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	void InteractiveAddPin();
	virtual void AddInputPin() override;
	virtual void RemoveInputPin(UEdGraphPin* Pin) override;

	void SwitchType();

	UEdGraphPin* GetInputPin();
	TArray<TPair<UEdGraphPin*, UEdGraphPin*>> GetFieldPins();

	UPROPERTY()
	bool bObjectOrArray = true;

	UPROPERTY()
	TArray<FJsonFieldMetadata> Fields;
};
