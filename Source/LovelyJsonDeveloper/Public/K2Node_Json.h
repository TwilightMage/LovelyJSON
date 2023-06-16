// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "LovelyJsonUtils.h"
#include "K2Node_Json.generated.h"

USTRUCT()
struct FJsonFieldMetadata
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FEdGraphPinType Type;
};

/**
 * 
 */
UCLASS()
class LOVELYJSONDEVELOPER_API UK2Node_Json : public UK2Node
{
	GENERATED_BODY()

public:
	virtual bool IsNodePure() const override { return true; }
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FLinearColor GetNodeTitleColor() const override;

	inline static const FEdGraphPinType WildcardPinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Wildcard, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	inline static const FEdGraphPinType StringPinType = FEdGraphPinType(UEdGraphSchema_K2::PC_String, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	inline static const FEdGraphPinType KeyPinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Name, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	inline static const FEdGraphPinType IndexPinType = FEdGraphPinType(UEdGraphSchema_K2::PC_Int, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	inline static const FEdGraphPinType JsonValPinType = FEdGraphPinType(ULovelyJsonUtils::PC_JsonPinCategory, NAME_None, nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	inline static const FEdGraphPinType JsonRefPinType = FEdGraphPinType(ULovelyJsonUtils::PC_JsonPinCategory, NAME_None, nullptr, EPinContainerType::None, true, FEdGraphTerminalType());
};
