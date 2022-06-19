// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicWildcardStoreType.generated.h"



UENUM()
enum class EDynamicWildcardStoreType : uint8
{
	/**  */
	Both   UMETA(DisplayName = "Both"),

	/** As binary, the data is stored and retrieved quicker. This data is smaller than a string version. */
	Binary  UMETA(DisplayName = "Binary"),

	/** As a string, the data is human readable.
	*/
	String   UMETA(DisplayName = "String"),
};