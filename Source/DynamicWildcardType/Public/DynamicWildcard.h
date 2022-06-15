// Copyright Dream Seed LLC, 2022

#pragma once

#include "CoreMinimal.h"
#include "DynamicWildcard.generated.h"

// Container for any type of data.
// Note: does not hold onto references of the data. So it may be garbage collected if only referenced by this struct.
USTRUCT(BlueprintType)
struct DYNAMICWILDCARDMODULE_API FDynamicWildcard
{
	GENERATED_BODY()
	
public:

	// The Property that holds the data. Useful for knowing what type of data we are pointing to.
	FProperty* ValueProperty;

	// Pointer to the actual data.
	void* ValuePointer;

	// Our value was last passed in directly as a string.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message System")
	bool bLastSetFromString = true;

	// Our value was given to us by wildcard value and had its string value cached into `ValueString`.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message System")
	bool bPointerHasBeenCachedToString = false;

	// This holds a string representation of the value, which is useful for marshaling data into anonymous function calls.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message System")
	FString ValueAsString;

	// TODO: This persistently holds reference to the object so the engine doesn't GC it.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "Message System")
	UObject* ValueAsObject;

	TArray<uint8> PropertySerialized;
	FArchive PropertyAsArchive;
	//FArchive ValueAsArchive;

	// TODO: Build array of all referenced UObjects referenced by ValuePointer if it is a Struct. 
	// This to prevent GC on anything referenced by the struct alone. Like an Array of UObjects.

	// TODO: Store the value in a byte array? Should we be serializing the data and then unserializing it?
	
};
