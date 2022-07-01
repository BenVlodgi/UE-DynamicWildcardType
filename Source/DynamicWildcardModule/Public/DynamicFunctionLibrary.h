// Copyright 2022 Dream Seed LLC.

#pragma once

#include "DynamicParametersStruct.h"
DECLARE_LOG_CATEGORY_EXTERN(LogDynamicReflection, Log, All);

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DynamicFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICWILDCARDMODULE_API UDynamicFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** Get all Interfaces this Class implements. **/
	UFUNCTION(BlueprintCallable, Category = "Dynamic Code Reflection")
	static void GetClassInterfaces(UClass* Class, TArray<UClass*>& outInterfaces);

	/** Get all function names from this Class/Interface. **/
	UFUNCTION(BlueprintCallable, Category = "Dynamic Code Reflection")
	static void GetFunctionNames(const UClass* Class, TArray<FName>& FunctionNames);

	/** Get all function names from this Class/Interface that have a specified keyword. **/
	UFUNCTION(BlueprintCallable, Category = "Dynamic Code Reflection", meta = (DevelopmentOnly))
	static void GetFunctionNamesWithKeyword(const UClass* Class, FString keyword, TArray<FName>& FunctionNames);

	/** Get all function names from this Class/Interface that have a specified keyword. **/
	UFUNCTION(BlueprintCallable, Category = "Dynamic Code Reflection", meta = (KeyWords = "property, args, argument"))
	static void GetFunctionParameterNames(const UClass* Class, FName FunctionName, TArray<FName>& ParameterNames);

	/**
	* Execute the function on target object with given parameters.
	* 
	* @param Target The target to call the function on.
	* @param FunctionName The Function to call.
	* @param Parameters Parameter values to pass into the function call.
	* @param bSuccess If the call was executed
	* @param ReturnValues Values returned from the function call.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dynamic Code Reflection", meta = (DisplayName = "Call Function by Name", KeyWords = "function, event, reflection, parameters, args, arguments"))
	static void CallFunctionByName(UObject* Target, const FName FunctionName, FDynamicParametersStruct Parameters, bool& bSuccess, FDynamicParametersStruct& ReturnValues);

};
