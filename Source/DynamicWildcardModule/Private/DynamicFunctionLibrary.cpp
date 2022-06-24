// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicFunctionLibrary.h"

#include "DynamicWildcardLibrary.h"

DEFINE_LOG_CATEGORY(LogDynamicReflection);

void UDynamicFunctionLibrary::GetClassInterfaces(UClass* Class, TArray<UClass*>& Interfaces)
{
	Interfaces.Empty();

	if (Class)
	{
		//return Class->Interfaces
		for (FImplementedInterface Interface : Class->Interfaces)
		{
			Interfaces.Add(Interface.Class);
		}
	}
}

void UDynamicFunctionLibrary::GetFunctionNames(const UClass* Class, TArray<FString>& FunctionNames)
{
	FunctionNames.Empty();
	//for (TFieldIterator<UFunction> FIT(Class, EFieldIteratorFlags::IncludeSuper); FIT; ++FIT)
	for (TFieldIterator<UFunction> FIT(Class); FIT; ++FIT)
	{
		UFunction* Function = *FIT;
		FunctionNames.Add(Function->GetName());
	}
}

void UDynamicFunctionLibrary::GetFunctionNamesWithKeyword(const UClass* Class, FString keyword, TArray<FString>& FunctionNames)
{
#if WITH_EDITORONLY_DATA //HasMetaData is editor only
	FunctionNames.Empty();
	for (TFieldIterator<UFunction> FIT(Class); FIT; ++FIT)
	{
		UFunction* Function = *FIT;
		if (Function->HasMetaData(TEXT("KeyWords")))
		{
			FString FunctionKeyWords = *Function->FindMetaData(TEXT("KeyWords"))->ToLower();
			if (FunctionKeyWords.Contains(keyword.ToLower()))
			{
				FunctionNames.Add(Function->GetName());
			}
		}
	}
#endif
}

void UDynamicFunctionLibrary::GetFunctionProperies(const UClass* Class, FString FunctionName, TArray<FString>& PropertyNames)
{
	PropertyNames.Empty();
	for (TFieldIterator<UFunction> FIT(Class); FIT; ++FIT)
	{
		UFunction* Function = *FIT;
		if (Function->GetName() == FunctionName)
		{
			for (TFieldIterator<FProperty>PIT(Function); PIT; ++PIT)
			{
				PropertyNames.Add(PIT->GetName());
			}
		}
	}
}

void UDynamicFunctionLibrary::CallFunctionByName(UObject* Target, const FName FunctionName, FDynamicParametersStruct Parameters, bool& bSuccess, FDynamicParametersStruct& ReturnValues)
{
	//const TCHAR* Str = TEXT("");
	FOutputDevice& Ar = *GLog;
	UObject* Executor = NULL;
	bool bForceCallWithNonExec = true;
	//bool UObject::CallFunctionByNameWithArguments(const TCHAR * Str, FOutputDevice & Ar, UObject * Executor, bool bForceCallWithNonExec/*=false*/)
	{
		//// Find an exec function.
		//FString MsgStr;
		//if (!FParse::Token(Str, MsgStr, true))
		//{
		//	UE_LOG(LogDynamicReflection, Verbose, TEXT("CallFunctionByName: Not Parsed '%s'"), Str);
		//	return false;
		//}
		//const FName FunctionName = FName(*MsgStr, FNAME_Find);
		//if (FunctionName == NAME_None)
		//{
		//	UE_LOG(LogDynamicReflection, Verbose, TEXT("CallFunctionByName: Name not found '%s'"), Str);
		//	return false;
		//}
		UFunction* Function = Target->FindFunction(FunctionName);
		if (nullptr == Function)
		{
			UE_LOG(LogDynamicReflection, Verbose, TEXT("CallFunctionByName: Function not found '%s'"), *FunctionName.ToString());
			bSuccess = false;
			return;
		}
		if (0 == (Function->FunctionFlags & FUNC_Exec) && !bForceCallWithNonExec)
		{
			UE_LOG(LogDynamicReflection, Verbose, TEXT("CallFunctionByName: Function not executable '%s'"), *FunctionName.ToString());

			bSuccess = false;
			return;
		}

		FProperty* LastParameter = nullptr;

		// find the last parameter
		for (TFieldIterator<FProperty> It(Function); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It)
		{
			LastParameter = *It;
		}

		// Parse all function parameters.
		uint8* Params = (uint8*)FMemory_Alloca(Function->ParmsSize); // UE4
		//uint8* Params = (uint8*)FMemory_Alloca_Aligned(Function->ParmsSize, Function->GetMinAlignment()); // UE5
		FMemory::Memzero(Params, Function->ParmsSize);

		for (TFieldIterator<FProperty> It(Function); It && It->HasAnyPropertyFlags(CPF_Parm); ++It)
		{
			FProperty* LocalProp = *It;
			checkSlow(LocalProp);
			if (!LocalProp->HasAnyPropertyFlags(CPF_ZeroConstructor))
			{
				LocalProp->InitializeValue_InContainer(Params);
			}
		}

		const uint32 ExportFlags = PPF_None;
		bool bFailed = false;
		int32 NumParamsEvaluated = 0;
		for (TFieldIterator<FProperty> It(Function); It && (It->PropertyFlags & (CPF_Parm | CPF_ReturnParm)) == CPF_Parm; ++It, NumParamsEvaluated++)
		{
			FProperty* PropertyParam = *It;
			checkSlow(PropertyParam); // Fix static analysis warning
			if (NumParamsEvaluated == 0 && Executor)
			{
				FObjectPropertyBase* Op = CastField<FObjectPropertyBase>(*It);
				if (Op && Executor->IsA(Op->PropertyClass))
				{
					// First parameter is implicit reference to object executing the command.
					Op->SetObjectPropertyValue(Op->ContainerPtrToValuePtr<uint8>(Params), Executor);
					continue;
				}
			}

			//// Keep old string around in case we need to pass the whole remaining string
			//const TCHAR* RemainingStr = Str;
			//
			//// Parse a new argument out of Str
			//FString ArgStr;
			//FParse::Token(Str, ArgStr, true);

			// if ArgStr is empty but we have more params to read parse the function to see if these have defaults, if so set them
			bool bFoundDefault = false;
			bool bFailedImport = true;


#if WITH_EDITOR
			// Don't do this now that we're executing. If ever do this, do it somehow on compile to collect the defaults. Also want Blueprint Defaults
			//if (false) 
			//{
			//	if (!FCString::Strcmp(*ArgStr, TEXT("")))
			//	{
			//		const FName DefaultPropertyKey(*(FString(TEXT("CPP_Default_")) + PropertyParam->GetName()));
			//		const FString& PropertyDefaultValue = Function->GetMetaData(DefaultPropertyKey);
			//		if (!PropertyDefaultValue.IsEmpty())
			//		{
			//			bFoundDefault = true;
			//
			//			const TCHAR* Result = It->ImportText(*PropertyDefaultValue, It->ContainerPtrToValuePtr<uint8>(Params), ExportFlags, NULL);
			//			bFailedImport = (Result == nullptr);
			//		}
			//	}
			//}
#endif

			{
				FName Key = FName(PropertyParam->GetName());
				FDynamicWildcard* MessageParameterValue = Parameters.Parameters.Find(Key);
				if (MessageParameterValue)
				{
					UE_LOG(LogDynamicReflection, Log, TEXT("CallFunctionByName: Found Property: %s"), *Key.ToString());

					auto* dataPointer = It->ContainerPtrToValuePtr<uint8>(Params);

					/*
					{
						// Use Value as String
						const TCHAR* Result = It->ImportText(*MessageParameterValue->ValueAsString, dataPointer, ExportFlags, NULL);
						bFailedImport = (Result == nullptr);
					}//*/
					{
						// Use Serialized Value
						// See UDynamicWildcardLibrary::execGetDynamicWildcard
						It->ClearValue_InContainer(dataPointer); // should do this for whole array if exists, right?
						bool bCompatiblePropertyType;
						UDynamicWildcardLibrary::CopyDynamicWildcardToProperty(*MessageParameterValue, *It, dataPointer, bCompatiblePropertyType);
						if (bCompatiblePropertyType)
						{
							bFailedImport = false;
						}
						//if (MessageParameterValue->PropertySerialized.IsValidIndex(0))
						//{
						//	It->CopyCompleteValue(dataPointer, MessageParameterValue->PropertySerialized.GetData());
						//	bFailedImport = false;
						//}
					}
				}
				else
				{
					UE_LOG(LogDynamicReflection, Log, TEXT("CallFunctionByName: Not found Property: %s"), *Key.ToString());
				}
			}


			//if (!bFoundDefault)
			//{
			//	// if this is the last string property and we have remaining arguments to process, we have to assume that this
			//	// is a sub-command that will be passed to another exec (like "cheat giveall weapons", for example). Therefore
			//	// we need to use the whole remaining string as an argument, regardless of quotes, spaces etc.
			//	if (PropertyParam == LastParameter && PropertyParam->IsA<FStrProperty>() && FCString::Strcmp(Str, TEXT("")) != 0)
			//	{
			//		ArgStr = FString(RemainingStr).TrimStart();
			//	}
			//
			//	const TCHAR* Result = It->ImportText(*ArgStr, It->ContainerPtrToValuePtr<uint8>(Params), ExportFlags, NULL);
			//	bFailedImport = (Result == nullptr);
			//}

			if (bFailedImport)
			{
				FFormatNamedArguments Arguments;
				Arguments.Add(TEXT("FunctionName"), FText::FromName(FunctionName));
				Arguments.Add(TEXT("PropertyName"), FText::FromName(It->GetFName()));
				Arguments.Add(TEXT("FunctionName"), FText::FromName(Function->GetFName()));
				Ar.Logf(TEXT("%s"), *FText::Format(NSLOCTEXT("Core", "BadProperty", "'{FunctionName}': Bad or missing property '{PropertyName}' when trying to call {FunctionName}"), Arguments).ToString());
				bFailed = true;

				continue;
			}
		}

		//if (!bFailed)
		{
			Target->ProcessEvent(Function, Params);
		}

		for (TFieldIterator<FProperty> It(Function); It; ++It)
		{

			if ((It->PropertyFlags & CPF_OutParm) == CPF_OutParm)
			{
				FString PropertyName;
				It->GetName(PropertyName);
				auto* dataPointer = It->ContainerPtrToValuePtr<uint8>(Params);

				FDynamicWildcard functionReturnValue = UDynamicWildcardLibrary::MakeDynamicWildcardFromProperty(*It, dataPointer);

				ReturnValues.Parameters.Add(FName(PropertyName), functionReturnValue);
			}

		}

		//!!destructframe see also UObject::ProcessEvent
		for (TFieldIterator<FProperty> It(Function); It && It->HasAnyPropertyFlags(CPF_Parm); ++It)
		{
			It->DestroyValue_InContainer(Params);
		}

		// Success.
		bSuccess = true;
		return;
	}
	bSuccess = false;
	return;
}
