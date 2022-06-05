// Copyright Dream Seed LLC, 2022

#pragma once

#include "DynamicWildcard.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DynamicWildcardLibrary.generated.h"

// Utilities for storing and retrieving data from the Dynamic Wildcard struct.
UCLASS(BlueprintType)
class DYNAMICWILDCARDTYPE_API UDynamicWildcardLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "Make Dynamic Wildcard", CustomStructureParam = "Value"), Category = "Dynamic Wildcard")
	static void MakeDynamicWildcard(const int32& Value, FDynamicWildcard& DynamicWildcard);
	DECLARE_FUNCTION(execMakeDynamicWildcard);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Get Value", CustomStructureParam = "Value"), Category = "Dynamic Wildcard")
	static void GetDynamicWildcard(const FDynamicWildcard& Target, int32& Value, bool& IsValid);
	DECLARE_FUNCTION(execGetDynamicWildcard);

	/** Converts the value stored in a MessageParameterValueStruct to a string. The value is cached for quicker retrieval in the future. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (Dynamic Wildcard)", CompactNodeTitle = "->", BlueprintAutocast), Category = "Dynamic Wildcard")
	static FString Conv_DynamicWildcardToString(UPARAM(ref) FDynamicWildcard& DynamicWildcard);

	//TODO: Add "LoadFromString" function that can reverse the ToString function. Utilizing the FProperty::ImportText function.

	//TODO: Add ToString(Wildcard) function. For fun? This would be better than making a macro that takes in wildcard and converts
	//      to dynamic wildcard, because macro wildcards have to know if they are arrays or not up-front.
};
