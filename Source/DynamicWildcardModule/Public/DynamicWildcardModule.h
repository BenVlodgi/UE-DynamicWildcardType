// Copyright 2022 Dream Seed LLC.

#pragma once

#include "Modules/ModuleManager.h"

class FDynamicWildcardModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegisterSettings();
	void UnregisterSettings();
};
