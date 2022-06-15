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


	Z_Param_DynamicWildcard.ValueProperty = ValueProperty;
	Z_Param_DynamicWildcard.ValuePointer = ValuePropertyAddress;
	Z_Param_DynamicWildcard.bLastSetFromString = false;
	Z_Param_DynamicWildcard.bPointerHasBeenCachedToString = false;
	//Z_Param_DynamicWildcard.ValueAsString = 
	UDynamicWildcardLibrary::Conv_DynamicWildcardToString(Z_Param_DynamicWildcard); //This will cache the value in ValueAsString;

	//Z_Param_DynamicWildcard.PropertyAsArchive = FArchive();
	//ValueProperty->Serialize(Z_Param_DynamicWildcard.PropertyAsArchive);


	//FMemoryWriter PropertySerializedWriter(Z_Param_DynamicWildcard.PropertySerialized);
	//ValueProperty->Serialize(PropertySerializedWriter);


	Z_Param_DynamicWildcard.PropertySerialized.SetNumUninitialized(Z_Param_DynamicWildcard.ValueProperty->GetSize());
	Z_Param_DynamicWildcard.ValueProperty->CopyCompleteValue(Z_Param_DynamicWildcard.PropertySerialized.GetData(), Z_Param_DynamicWildcard.ValuePointer);


	//ValueProperty->SerializeItem(Z_Param_DynamicWildcard.ValueAsArchive);

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

	P_NATIVE_END;
}



DEFINE_FUNCTION(UDynamicWildcardLibrary::execGetDynamicWildcard)
{
	// Retrieves Target parameter from the stack.
	P_GET_STRUCT_REF(FDynamicWildcard, Z_Param_Target);

	// Retrieves IsValid bool reference parameter from the stack. This must come now before the wildcard parmeter because of property stepping order.
	P_GET_UBOOL_REF(Z_Param_Out_IsValid);

	FProperty* InValueProperty = Z_Param_Target.ValueProperty;
	void* InValue = Z_Param_Target.ValuePointer;

	Stack.MostRecentPropertyAddress = NULL;

	FProperty* OutValueProperty = NULL;
	void* OutValuePointer = NULL;

	//FProperty* OutDummyProperty = NULL;
	//void* OutDummyPointer = NULL;

	if (Stack.Code)
	{
		Stack.Step(Stack.Object, OutValuePointer);
	}
	else
	{
		// Does this ever happen?
		UE_LOG(LogTemp, Error, TEXT("Dynamic Wildcard: Get: False: Stack.Code: %d"), Stack.Code);
		checkSlow(CastField<TProperty>(PropertyChainForCompiledIn) && CastField<FProperty>(PropertyChainForCompiledIn));
		FProperty* Property = (FProperty*)Stack.PropertyChainForCompiledIn;
		Stack.PropertyChainForCompiledIn = Property->Next;
		Stack.StepExplicitProperty(OutValuePointer, Property);

	}
	OutValueProperty = Stack.MostRecentProperty;
	OutValuePointer = Stack.MostRecentPropertyAddress;

	// Check if this is the right way
	//Stack.Step(Stack.Object, OutValuePointer);
	//OutDummyProperty = Stack.MostRecentProperty;


	P_FINISH;
	P_NATIVE_BEGIN;


	bool bCompatiblePropertyType = false;

	/*
	bool bCopyFromPointer = true;
	if(bCopyFromPointer)
	{
		FProperty* OutValueProperty = CastField<FProperty>(Stack.MostRecentProperty);
		if (InValueProperty && InValue && OutValueProperty)
		{
			if (InValueProperty->GetClass()->IsChildOf(OutValueProperty->GetClass()))
			{
				// This uses the pointers
				OutValueProperty->CopyCompleteValueFromScriptVM((Stack.MostRecentPropertyAddress != NULL) ? (void*)(Stack.MostRecentPropertyAddress) : (void*)OutValuePointer, InValue);
				bCompatiblePropertyType = true;
			}
		}
	}//*/

	/*
	bool bDeserializeFromString = true;
	if(bDeserializeFromString)
	{
		//Note: If used, Should this have a type check ahead of time?
		// This Deserializes the data from a string.
		FProperty* OutValueProperty = CastField<FProperty>(Stack.MostRecentProperty);
		const FString TargetValueAsString = UDynamicWildcardLibrary::Conv_DynamicWildcardToString(Z_Param_Target);
		const TCHAR* Result = OutValueProperty->ImportText(*TargetValueAsString, Stack.MostRecentPropertyAddress, PPF_None, NULL);
		bCompatiblePropertyType = Result != NULL;
	}//*/

	///*
	bool bDeserializeFromBinary = true;
	if (bDeserializeFromBinary)
	{
		//FProperty* OutValueProperty = CastField<FProperty>(Stack.MostRecentProperty);
		//uint8* OutValuePointer = Stack.MostRecentPropertyAddress;

		OutValueProperty->CopyCompleteValue(OutValuePointer, Z_Param_Target.PropertySerialized.GetData());

		//FMemoryReader PropertySerializedReader(Z_Param_Target.PropertySerialized);
		////UField field = UField(EStaticConstructor::EC_StaticConstructor, EObjectFlags::RF_Dynamic);
		////FProperty DeserializedProperty = FProperty::GetDefaultPropertyValue();
		////DeserializedProperty.Serialize(PropertySerializedReader);
		////DeserializedProperty.CopyCompleteValueToScriptVM((Stack.MostRecentPropertyAddress != NULL) ? (void*)(Stack.MostRecentPropertyAddress) : (void*)OutValuePointer, InValue);
		//
		////OutValueProperty->CopyCompleteValueFromScriptVM((Stack.MostRecentPropertyAddress != NULL) ? (void*)(Stack.MostRecentPropertyAddress) : &Z_Param_Target.PropertySerialized, InValue);
		////OutValueProperty->CopyCompleteValueFromScriptVM(Stack.MostRecentPropertyAddress, &Z_Param_Target.PropertySerialized);
		//OutValueProperty->Serialize(PropertySerializedReader);
		
		bCompatiblePropertyType = true;

		

		//FStrProperty::TCppType A = FStrProperty::GetDefaultPropertyValue();

		//UObject::execInstanceVariable()
		
		
		// P_GET_PROPERTY
		//UIntProperty::TCppType Z_Param_a = UIntProperty::GetDefaultPropertyValue();
		//Stack.StepCompiledIn < UIntProperty >(&Z_Param_a);



		//// Copy value from deserialized pointer to ouptut.
		//FProperty* DeserializedProperty = Z_Param_Target.PropertyAsArchive.GetSerializedProperty();
		//if (DeserializedProperty)
		//{
		//	DeserializedProperty->CopyCompleteValueToScriptVM((Stack.MostRecentPropertyAddress != NULL) ? (void*)(Stack.MostRecentPropertyAddress) : (void*)OutValuePointer, InValue);
		//	bCompatiblePropertyType = true;
		//}
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