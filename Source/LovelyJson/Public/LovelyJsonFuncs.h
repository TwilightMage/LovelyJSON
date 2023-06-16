// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "JsonObjectConverter.h"
#include "Dom/JsonValue.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "LovelyJsonFuncs.generated.h"

UENUM(BlueprintType)
enum class EJsonValueType
{
	Null,
	String,
	Number,
	Boolean,
	Object,
	Array
};

USTRUCT()
struct FJsonValueContainer
{
	GENERATED_BODY()

	TSharedPtr<FJsonValue> Value;
};

USTRUCT()
struct FDecoyStruct
{
	GENERATED_BODY()
};

#define SET_PROPERTY_SINGLE(Type, ValueDef)                    \
TSharedPtr<FJsonObject>* Obj;                                  \
if(Target.Value->TryGetObject(Obj))                            \
{                                                              \
	(*Obj)->Set##Type##Field(PropName.ToString(), (ValueDef)); \
}                                                              \

#define SET_PROPERTY_ARRAY(Type, ValueDef)                            \
TSharedPtr<FJsonObject>* Obj;                                         \
if(Target.Value->TryGetObject(Obj))                                   \
{                                                                     \
	TArray<TSharedPtr<FJsonValue>> JsonArray;                         \
	JsonArray.Reserve(Array.Num());                                   \
	for (const auto& Item : Array)                                    \
	{                                                                 \
		JsonArray.Add(MakeShared<FJsonValue##Type>(ValueDef));        \
	}                                                                 \
	(*Obj)->SetArrayField(PropName.ToString(), JsonArray);            \
};                                                                    \

UCLASS(Abstract)
class LOVELYJSON_API ULovelyJsonFuncs : public UObject
{
	GENERATED_BODY()

public:
	// Basic
	
	UFUNCTION()
	static FJsonValueContainer CreateObject()
	{
		FJsonValueContainer Result;
		Result.Value = MakeShared<FJsonValueObject>(MakeShared<FJsonObject>());
		return Result;
	}

	// Strings
	
	UFUNCTION()
	static void SetProperty_single_text(FJsonValueContainer& Target, const FName& PropName, const FText& Item)
	{ SET_PROPERTY_SINGLE(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_array_text(FJsonValueContainer& Target, const FName& PropName, const TArray<FText>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }

	UFUNCTION()
	static void SetProperty_single_string(FJsonValueContainer& Target, const FName& PropName, const FString& Item)
	{ SET_PROPERTY_SINGLE(String, Item); }
	UFUNCTION()
	static void SetProperty_array_string(FJsonValueContainer& Target, const FName& PropName, const TArray<FString>& Array)
	{ SET_PROPERTY_ARRAY(String, Item); }
	UFUNCTION()
	static void SetProperty_set_string(FJsonValueContainer& Target, const FName& PropName, const TSet<FString>& Array)
	{ SET_PROPERTY_ARRAY(String, Item); }

	UFUNCTION()
	static void SetProperty_single_name(FJsonValueContainer& Target, const FName& PropName, const FName& Item)
	{ SET_PROPERTY_SINGLE(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_array_name(FJsonValueContainer& Target, const FName& PropName, const TArray<FName>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_set_name(FJsonValueContainer& Target, const FName& PropName, const TSet<FName>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }

	// Boolean

	UFUNCTION()
	static void SetProperty_single_bool(FJsonValueContainer& Target, const FName& PropName, bool Array)
	{ SET_PROPERTY_SINGLE(Bool, Array); }
	UFUNCTION()
	static void SetProperty_array_bool(FJsonValueContainer& Target, const FName& PropName, const TArray<bool>& Array)
	{ SET_PROPERTY_ARRAY(Boolean, Item); }
	UFUNCTION()
	static void SetProperty_set_bool(FJsonValueContainer& Target, const FName& PropName, const TSet<bool>& Array)
	{ SET_PROPERTY_ARRAY(Boolean, Item); }

	// Numeric

	UFUNCTION()
	static void SetProperty_single_byte(FJsonValueContainer& Target, const FName& PropName, int8 Item)
	{ SET_PROPERTY_SINGLE(Number, Item); }
	UFUNCTION()
	static void SetProperty_array_byte(FJsonValueContainer& Target, const FName& PropName, TArray<int8> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }
	UFUNCTION()
	static void SetProperty_set_byte(FJsonValueContainer& Target, const FName& PropName, TSet<int8> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }

	UFUNCTION()
	static void SetProperty_single_int(FJsonValueContainer& Target, const FName& PropName, int32 Item)
	{ SET_PROPERTY_SINGLE(Number, Item); }
	UFUNCTION()
	static void SetProperty_array_int(FJsonValueContainer& Target, const FName& PropName, TArray<int32> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }
	UFUNCTION()
	static void SetProperty_set_int(FJsonValueContainer& Target, const FName& PropName, TSet<int32> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }

	UFUNCTION()
	static void SetProperty_single_int64(FJsonValueContainer& Target, const FName& PropName, int32 Item)
	{ SET_PROPERTY_SINGLE(Number, Item); }
	UFUNCTION()
	static void SetProperty_array_int64(FJsonValueContainer& Target, const FName& PropName, TArray<int64> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }
	UFUNCTION()
	static void SetProperty_set_int64(FJsonValueContainer& Target, const FName& PropName, TSet<int64> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }

	UFUNCTION()
	static void SetProperty_single_float(FJsonValueContainer& Target, const FName& PropName, float Item)
	{ SET_PROPERTY_SINGLE(Number, Item); }
	UFUNCTION()
	static void SetProperty_array_float(FJsonValueContainer& Target, const FName& PropName, TArray<float> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }
	UFUNCTION()
	static void SetProperty_set_float(FJsonValueContainer& Target, const FName& PropName, TSet<float> Array)
	{ SET_PROPERTY_ARRAY(Number, Item); }

	// Struct

	UFUNCTION()
	static void SetProperty_single_struct(FJsonValueContainer& Target, const FName& PropName, UScriptStruct* StructClass, const FLinearColor& Struct)
	{
		TSharedPtr<FJsonObject>* Obj;
		if (Target.Value->TryGetObject(Obj))
		{
			//TSharedPtr<FJsonObject> StructJson;
			//FJsonObjectConverter::UStructToJsonObject(StructClass, &Struct, StructJson.ToSharedRef());
			//(*Obj)->SetObjectField(PropName.ToString(), StructJson);
		};
	}
	
	static void SetProperty_single_struct_native(UObject* Context, FFrame& Stack, RESULT_DECL)
	{
		P_GET_STRUCT_REF(FJsonValueContainer, Target);
		P_GET_PROPERTY_REF(FNameProperty, PropName);
		P_GET_OBJECT(UScriptStruct, StructClass);
		P_GET_STRUCT_REF(FDecoyStruct, Struct);
		P_FINISH;
		P_NATIVE_BEGIN;
		
		TSharedPtr<FJsonObject>* Obj;
		if (Target.Value->TryGetObject(Obj))
		{
			//TSharedPtr<FJsonObject> StructJson;
			//FJsonObjectConverter::UStructToJsonObject(StructClass, &Struct, StructJson.ToSharedRef());
			//(*Obj)->SetObjectField(PropName.ToString(), StructJson);
		};
		
		P_NATIVE_END;
	}

	// Soft reference

	UFUNCTION()
	static void SetProperty_single_softclass(FJsonValueContainer& Target, const FName& PropName, const TSoftClassPtr<UObject>& Item)
	{ SET_PROPERTY_SINGLE(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_array_softclass(FJsonValueContainer& Target, const FName& PropName, const TArray<TSoftClassPtr<UObject>>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_set_softclass(FJsonValueContainer& Target, const FName& PropName, const TSet<TSoftClassPtr<UObject>>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }

	UFUNCTION()
	static void SetProperty_single_softobject(FJsonValueContainer& Target, const FName& PropName, const TSoftObjectPtr<UObject>& Item)
	{ SET_PROPERTY_SINGLE(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_array_softobject(FJsonValueContainer& Target, const FName& PropName, const TArray<TSoftObjectPtr<UObject>>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }
	UFUNCTION()
	static void SetProperty_set_softobject(FJsonValueContainer& Target, const FName& PropName, const TSet<TSoftObjectPtr<UObject>>& Array)
	{ SET_PROPERTY_ARRAY(String, Item.ToString()); }

	// Stringify

	UFUNCTION()
	static FString JsonStringify(const FJsonValueContainer& Json)
	{
		switch (Json.Value->Type)
		{
		case EJson::None:
			return "";
		case EJson::Null:
			return "null";
		case EJson::Array:
			{
				FString ArrayString;
				TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ArrayJsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&ArrayString, 0);
				FJsonSerializer::Serialize(Json.Value->AsArray(), ArrayJsonWriter, true);
				return ArrayString;
			}
		case EJson::Object:
			{
				FString ObjectString;
				TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> ObjectJsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&ObjectString, 0);
				FJsonSerializer::Serialize(Json.Value->AsObject().ToSharedRef(), ObjectJsonWriter, true);
				return ObjectString;
			}
		default:
			return Json.Value->AsString();
		}
	}

	UFUNCTION()
	static FJsonValueContainer ParseJson(const FString& String)
	{
		return FJsonValueContainer();
	}
};
