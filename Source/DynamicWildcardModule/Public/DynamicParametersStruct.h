// Copyright 2022 Dream Seed LLC.

#pragma once

#include "DynamicWildcard.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DynamicParametersStruct.generated.h"

// 
USTRUCT(BlueprintType)
struct DYNAMICWILDCARDMODULE_API FDynamicParametersStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic|Function")
	TMap<FName, FDynamicWildcard> Parameters;
};
