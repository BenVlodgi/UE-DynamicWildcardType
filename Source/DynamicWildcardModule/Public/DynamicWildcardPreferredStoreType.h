// Copyright 2022 Dream Seed LLC.

#pragma once

#include "CoreMinimal.h"
#include "DynamicWildcardPreferredStoreType.generated.h"



UENUM()
enum class EDynamicWildcardPreferredStoreType : uint8
{
	/**  */
	Default   UMETA(DisplayName = "Default"),

	/** As binary, the data is stored and retrieved quicker. This data is smaller than a string version. */
	Binary  UMETA(DisplayName = "Binary"),

	/** As a string, the data is human readable.
	*/
	String   UMETA(DisplayName = "String"),
};