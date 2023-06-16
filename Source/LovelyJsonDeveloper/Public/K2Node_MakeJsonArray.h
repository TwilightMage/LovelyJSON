﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_Json.h"
#include "K2Node_MakeJsonArray.generated.h"

/**
 * 
 */
UCLASS()
class LOVELYJSONDEVELOPER_API UK2Node_MakeJsonArray : public UK2Node_Json, public IK2Node_AddPinInterface
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	//virtual FNodeHandlingFunctor* CreateNodeHandler(FKismetCompilerContext& CompilerContext) const override;
	//virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	//virtual void PinTypeChanged(UEdGraphPin* Pin) override;
	//virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;

	virtual void AddInputPin() override;
	virtual void RemoveInputPin(UEdGraphPin* Pin) override;

	UPROPERTY()
	int32 NumInputs = 1;
};
