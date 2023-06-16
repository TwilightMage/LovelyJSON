﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_Json.h"
#include "K2Node_CloneJson.generated.h"

/**
 * 
 */
UCLASS()
class LOVELYJSONDEVELOPER_API UK2Node_CloneJson : public UK2Node_Json
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;
	virtual FNodeHandlingFunctor* CreateNodeHandler(FKismetCompilerContext& CompilerContext) const override;

	UEdGraphPin* GetInputPin() const;
	UEdGraphPin* GetOutputPin() const;
};