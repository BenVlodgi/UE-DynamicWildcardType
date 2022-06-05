// Copyright Dream Seed LLC, 2022

#pragma once

#include "DynamicWildcardLibrary.h"


DEFINE_FUNCTION(UDynamicWildcardLibrary::execMakeDynamicWildcard)
{
	Stack.StepCompiledIn<FStructProperty>(NULL);
	void* ValuePropertyAddress = Stack.MostRecentPropertyAddress;
	FProperty* ValueProperty = Stack.MostRecentProperty;

	P_GET_STRUCT_REF(FDynamicWildcard, Z_Param_DynamicWildcard);
	Z_Param_DynamicWildcard.ValueProperty = ValueProperty;
	Z_Param_DynamicWildcard.ValuePointer = ValuePropertyAddress;
	Z_Param_DynamicWildcard.ValueString = "";
	Z_Param_DynamicWildcard.bLastSetFromString = false;
	Z_Param_DynamicWildcard.bPointerHasBeenCachedToString = false;

	P_FINISH;
	P_NATIVE_BEGIN;
	P_NATIVE_END;
}



DEFINE_FUNCTION(UDynamicWildcardLibrary::execGetDynamicWildcard)
{
	P_GET_STRUCT_REF(FDynamicWildcard, Z_Param_Target);
	FDynamicWildcard& Target = Z_Param_Target;

	FProperty* ValueProperty = Z_Param_Target.ValueProperty;
	void* Value = Z_Param_Target.ValuePointer;

	Stack.MostRecentPropertyAddress = NULL;
	void* Z_Param_Out_Value_Temp = nullptr;

	if (Stack.Code)
	{
		Stack.Step(Stack.Object, Z_Param_Out_Value_Temp);
	}
	else
	{
		checkSlow(CastField<TProperty>(PropertyChainForCompiledIn) && CastField<FProperty>(PropertyChainForCompiledIn));
		FProperty* Property = (FProperty*)Stack.PropertyChainForCompiledIn;
		Stack.PropertyChainForCompiledIn = Property->Next;
		Stack.StepExplicitProperty(Z_Param_Out_Value_Temp, Property);
	}

	FProperty* SrcProperty = CastField<FProperty>(Stack.MostRecentProperty);
	bool bCompatiblePropertyType = false;
	if (ValueProperty && Value && SrcProperty)
	{
		if (ValueProperty->GetClass()->IsChildOf(SrcProperty->GetClass()))
		{
			bCompatiblePropertyType = true;
			SrcProperty->CopyCompleteValueFromScriptVM((Stack.MostRecentPropertyAddress != NULL) ? (void*)(Stack.MostRecentPropertyAddress) : (void*)Z_Param_Out_Value_Temp, Value);
		}
	}

	P_GET_UBOOL_REF(Z_Param_Out_IsValid);
	Z_Param_Out_IsValid = bCompatiblePropertyType;

	P_FINISH;
	P_NATIVE_BEGIN;
	P_NATIVE_END;
}

FString UDynamicWildcardLibrary::Conv_DynamicWildcardToString(FDynamicWildcard& DynamicWildcard)
{
	FString returnValue;
	if (DynamicWildcard.bLastSetFromString || DynamicWildcard.bPointerHasBeenCachedToString)
	{
		returnValue = DynamicWildcard.ValueString;
	}
	else if (DynamicWildcard.ValueProperty)
	{
		DynamicWildcard.ValueProperty->ExportTextItem(returnValue, DynamicWildcard.ValuePointer, nullptr, nullptr, 0);
		DynamicWildcard.ValueString = returnValue;
		DynamicWildcard.bPointerHasBeenCachedToString = true;
	}

	if (returnValue.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("MessageSystem: Unable to convert MessageParameterValue to string: %s"), DynamicWildcard.ValueProperty);
		returnValue = TEXT("");
	}

	return returnValue;
}

//FMessageParameterValueStruct UDynamicWildcardLibrary::Conv_StringToMessageParameterValueStruct(FString String, EPropertyTypeEnum Type)
//{
//	FMessageParameterValueStruct returnValue;
//
//	returnValue.ValueString = String;
//
//	//switch (Type)
//	//{
//	//	//EmitTerm(Statement.LHS, (FProperty*)(GetDefault<FBoolProperty>()));
//	//	case EPropertyTypeEnum::None: returnValue.ParameterValueProperty = NULL; break;
//	//	case EPropertyTypeEnum::Bool: returnValue.ParameterValueProperty = Cast<FBoolProperty>(FBoolProperty::StaticClass()->GetDefaultObject()); break;
//	//	case EPropertyTypeEnum::Byte:
//	//	case EPropertyTypeEnum::Class:
//	//	case EPropertyTypeEnum::Enum:
//	//	case EPropertyTypeEnum::MulticastInlineDelegate:
//	//	case EPropertyTypeEnum::MulticastSparseDelegate: returnValue.ParameterValueProperty = NULL;
//	//	case EPropertyTypeEnum::Numeric: returnValue.ParameterValueProperty = Cast<FNumericProperty>(FNumericProperty::StaticClass()->GetDefaultObject()); break;
//	//	case EPropertyTypeEnum::Object:
//	//	case EPropertyTypeEnum::ObjectPropertyBase:
//	//	case EPropertyTypeEnum::SoftObject:
//	//	case EPropertyTypeEnum::Struct:
//	//	default: returnValue.ParameterValueProperty = NULL;
//	//}
//	//
//	//if (returnValue.ParameterValueProperty)
//	//{
//	//	const TCHAR* Result = returnValue.ParameterValueProperty->ImportText(*String, returnValue.ParameterValueProperty->ContainerPtrToValuePtr<uint8>(&returnValue), PPF_None, NULL);
//	//	if (!Result)
//	//	{
//	//		UE_LOG(LogTemp, Warning, TEXT("Conv_StringToMessageParameterValue Failed"));
//	//	}
//	//}
//	return returnValue;
//}
