// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DynamicWildcardStoreType.h"
#include "DynamicWildcardPreferredStoreType.h"

#include "DynamicWildcardPluginSettings.generated.h"


UCLASS(config = Engine, defaultconfig)
class DYNAMICWILDCARDMODULE_API UDynamicWildcardPluginSettings : public UObject
{
	GENERATED_BODY()
	
public:
	/**How the wildcard data is stored.*/
	UPROPERTY(config, EditAnywhere, Category = "Dynamic Wildcard", meta = (DisplayName = "Store Data Method"))
	EDynamicWildcardStoreType StoreDataMethod = EDynamicWildcardStoreType::Both;

	/**The wildcard data's preferred way of being loaded. If this way is unavailable the other way will be used.*/
	UPROPERTY(config, EditAnywhere, Category = "Dynamic Wildcard", meta = (DisplayName = "Preferred Data Retrieval Method"))
	EDynamicWildcardPreferredStoreType PreferredDataRetrievalMethod = EDynamicWildcardPreferredStoreType::Default;
	
	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Dynamic Wildcard Plugin Settings"))
	static FORCEINLINE UDynamicWildcardPluginSettings* Get()
	{
		UDynamicWildcardPluginSettings* Settings = GetMutableDefault<UDynamicWildcardPluginSettings>();
		check(Settings);

		return Settings;
	}
};
