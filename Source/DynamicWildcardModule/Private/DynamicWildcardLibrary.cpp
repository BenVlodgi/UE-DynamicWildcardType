// Copyright Dream Seed LLC, 2022

#pragma once

#include "DynamicWildcardLibrary.h"


DEFINE_FUNCTION(UDynamicWildcardLibrary::execMakeDynamicWildcard)
{
	Stack.StepCompiledIn<FStructProperty>(NULL);
	void* ValuePropertyAddress = Stack.MostRecentPropertyAddress;
	FProperty* ValueProperty = Stack.MostRecentProperty;

	P_GET_STRUCT_REF(FDynamicWildcard, Z_Param_DynamicWildcard);

	P_FINISH;
	P_NATIVE_BEGIN;

	Z_Param_DynamicWildcard = MakeDynamicWildcardFromProperty(ValueProperty, ValuePropertyAddress);
	

	P_NATIVE_END;
}

FDynamicWildcard UDynamicWildcardLibrary::MakeDynamicWildcardFromProperty(FProperty* ValueProperty, void* ValuePropertyAddress)
{
	//ValuePropertyAddress = nullptr;
	//ValuePropertyAddress = ValueProperty->value; TODO: ME
	FDynamicWildcard Z_Param_DynamicWildcard;

	//Z_Param_DynamicWildcard.ValueProperty = ValueProperty;
	//Z_Param_DynamicWildcard.ValuePointer = ValuePropertyAddress;
	Z_Param_DynamicWildcard.bLastSetFromString = false;

	bool bCacheValuesToString = true; // TODO Make this a plugin configuration.
	if (bCacheValuesToString)
	{
		Z_Param_DynamicWildcard.ValueAsString = ""; // Must clear it incase this is being called over and over in the same player. The struct ref is the same. And will stack the string value otherwise.
		ValueProperty->ExportTextItem(Z_Param_DynamicWildcard.ValueAsString, ValuePropertyAddress, nullptr, nullptr, 0);
		Z_Param_DynamicWildcard.bPointerHasBeenCachedToString = true;
	}
	else
	{
		Z_Param_DynamicWildcard.bPointerHasBeenCachedToString = false;
		UDynamicWildcardLibrary::Conv_DynamicWildcardToString(Z_Param_DynamicWildcard); //This will cache the value in ValueAsString;
	}


	Z_Param_DynamicWildcard.PropertySerialized.SetNumUninitialized(ValueProperty->GetSize());
	ValueProperty->CopyCompleteValue(Z_Param_DynamicWildcard.PropertySerialized.GetData(), ValuePropertyAddress);


	// Attempts to get reference to the pointer value as an object.
	{
		//Z_Param_DynamicWildcard.ValueAsObject =

		//UObject* ValueAsObject;
		//ValueAsObject = (UObject*)ValuePropertyAddress;
		////ValueAsObject = dynamic_cast<UObject*>(ValuePropertyAddress);
		//ValueAsObject = reinterpret_cast<UObject*>(ValuePropertyAddress);
		////ValueAsObject = Cast<UObject*>(ValuePropertyAddress);
		//
		//if (ValueAsObject && IsValid(ValueAsObject))
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Dynamic Wildcard: Able to convert Dynamic Wildcard to object: %s."), *ValueAsObject->GetPathName());
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Dynamic Wildcard: Unable to convert Dynamic Wildcard to object."));
		//}


		//UObject* Ptr;
		////Ptr = ((UObject*)voidPtr)->IsA(someClass) ? (UObject*)voidPtr : nullptr;
		//Ptr = dynamic_cast<UObject*>(ValuePropertyAddress);
	}

	return Z_Param_DynamicWildcard;
}

void UDynamicWildcardLibrary::CopyDynamicWildcardToProperty(FDynamicWildcard DynamicWildcard, FProperty* ValueProperty, void* ValuePropertyAddress)
{
	// TODO: Implement
}

DEFINE_FUNCTION(UDynamicWildcardLibrary::execGetDynamicWildcard)
{
	// Retrieves Target parameter from the stack.
	P_GET_STRUCT_REF(FDynamicWildcard, Z_Param_Target);

	// Retrieves IsValid bool reference parameter from the stack. This must come now before the wildcard parmeter because of property stepping order.
	P_GET_UBOOL_REF(Z_Param_Out_IsValid);

	Stack.MostRecentPropertyAddress = NULL;
	FProperty* OutValueProperty = NULL;
	void* OutValuePointer = NULL;

	//Stack.StepCompiledInRef(OutValuePointer);
	Stack.Step(Stack.Object, OutValuePointer);
	

	OutValueProperty = Stack.MostRecentProperty;
	OutValuePointer = Stack.MostRecentPropertyAddress;


	P_FINISH;
	P_NATIVE_BEGIN;


	bool bCompatiblePropertyType = false;

	/* 
	//bool bDeserializeFromString = true;
	//if(bDeserializeFromString)
	{
		// Deserialize From String
		//Note: If used, Should this have a type check ahead of time?
		// This Deserializes the data from a string.
		FProperty* OutValueProperty = CastField<FProperty>(Stack.MostRecentProperty);
		const FString TargetValueAsString = UDynamicWildcardLibrary::Conv_DynamicWildcardToString(Z_Param_Target);
		const TCHAR* Result = OutValueProperty->ImportText(*TargetValueAsString, Stack.MostRecentPropertyAddress, PPF_None, NULL);
		bCompatiblePropertyType = Result != NULL;
	}//*/

	///*
	//bool bDeserializeFromBinary = true;
	//if (bDeserializeFromBinary)
	{
		// Deserialize From Binary
		OutValueProperty->ClearValue_InContainer(OutValuePointer); // should do this for whole array if exists, right?
		if (Z_Param_Target.PropertySerialized.IsValidIndex(0))
		{
			OutValueProperty->CopyCompleteValue(OutValuePointer, Z_Param_Target.PropertySerialized.GetData());
			bCompatiblePropertyType = true;
		}
		else
		{
			// No serialized value
			bCompatiblePropertyType = false;
		}
	}//*/

	Z_Param_Out_IsValid = bCompatiblePropertyType;


	P_NATIVE_END;
}

FString UDynamicWildcardLibrary::Conv_DynamicWildcardToString(FDynamicWildcard& DynamicWildcard)
{
	FString returnValue;
	if (DynamicWildcard.bLastSetFromString || DynamicWildcard.bPointerHasBeenCachedToString)
	{
		returnValue = DynamicWildcard.ValueAsString;
	}
	else if (DynamicWildcard.ValueProperty)
	{
		DynamicWildcard.ValueProperty->ExportTextItem(returnValue, DynamicWildcard.ValuePointer, nullptr, nullptr, 0);
		DynamicWildcard.ValueAsString = returnValue;
		DynamicWildcard.bPointerHasBeenCachedToString = true;
	}

	if (returnValue.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Dynamic Wildcard: Unable to convert Dynamic Wildcard to string: %s"), DynamicWildcard.ValueProperty);
		returnValue = TEXT("");
	}

	return returnValue;
}

//FDynamicWildcard UDynamicWildcardLibrary::Conv_StringToMessageParameterValueStruct(FString String, EPropertyTypeEnum Type)
//{
//	FDynamicWildcard returnValue;
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




//#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
//
//bool UDynamicWildcardLibrary::SaveObjectToMemory(UObject* Object, TArray<uint8>& OutSaveData)
//{
//	if (Object)
//	{
//		FMemoryWriter MemoryWriter(OutSaveData, true);
//
//		// Then save the object state, replacing object refs and names with strings
//		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
//		Object->Serialize(Ar);
//
//		return true;
//	}
//
//	return false;
//}