// Copyright 2022 Dream Seed LLC.

#include "DynamicWildcardModule.h"

#include "DynamicWildcardPluginSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FDynamicWildcardModule"

void FDynamicWildcardModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	RegisterSettings();
}

void FDynamicWildcardModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

void FDynamicWildcardModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		UDynamicWildcardPluginSettings* PluginSettings = GetMutableDefault<UDynamicWildcardPluginSettings>();

		SettingsModule->RegisterSettings("Project", "Plugins", "Dynamic Wildcard",
			LOCTEXT("RuntimeSettingsName", "Dynamic Wildcard"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Dynamic Wildcard plugin"),
			PluginSettings
		);
	}
}

void FDynamicWildcardModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Dynamic Wildcard");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDynamicWildcardModule, DynamicWildcardModule)