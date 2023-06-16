#include "LovelyJsonUtils.h"

#include "LovelyJsonFuncs.h"

ULovelyJsonUtils::ULovelyJsonUtils ()
{
	if (!CreateObjectFunc) CreateObjectFunc = ULovelyJsonFuncs::StaticClass()->ClassDefaultObject->FindFunction("CreateObject");
	
	if (FuncMap.IsEmpty())
	{
		const static TArray<FName> ContainerTypes = {
			"single",
			"array",
			"set",
			"map"
		};
		
		const static TArray<FName> PropertyTypes = {
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

		for (int8 ContainerType = 0; ContainerType < 4; ContainerType++)
		{
			auto& ContainerFuncs = FuncMap.Add((EPinContainerType)ContainerType);

			//UFunction* SetProperty_single_struct_Func = NewObject<UFunction>(UJsonFuncs::StaticClass(), "SetProperty_single_struct", RF_MarkAsRootSet);
			//SetProperty_single_struct_Func->NumParms = 4;
			//SetProperty_single_struct_Func->ParmsSize = 4 + sizeof(FJsonValueContainer&) + sizeof(FName) + sizeof(UScriptStruct*) + sizeof(FDecoyStruct&);
			//SetProperty_single_struct_Func->ReturnValueOffset = MAX_uint16;
			//SetProperty_single_struct_Func->SetNativeFunc(&UJsonFuncs::SetProperty_single_struct_native);
			//SetProperty_single_struct_Func->FunctionFlags |= FUNC_Final | FUNC_Native | FUNC_Static | FUNC_Public | FUNC_HasOutParms;
			//ContainerFuncs.Add("struct", SetProperty_single_struct_Func);
			
			for (const auto& Type : PropertyTypes)
			{
				auto& Func = ContainerFuncs.FindOrAdd(Type, nullptr);
				if (Func) continue;
				
				const auto FuncName = FName(FString::Printf(TEXT("SetProperty_%s_%s"), *ContainerTypes[ContainerType].ToString(), *Type.ToString()));
				Func = ULovelyJsonFuncs::StaticClass()->ClassDefaultObject->FindFunction(FuncName);

				if (!Func)
				{
					UE_LOG(LogTemp, Error, TEXT("JSON object function was not found: %s"), *FuncName.ToString())
				}
			}
		}
	}
}
