#pragma once
#include "SDK.h"

void InitKeyValues(KeyValues* keyValues, const char* name);

void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer);

void ForceMaterial(Color color, IMaterial* material, bool useColor = true, bool forceMaterial = true);


IMaterial* CreateMaterial(bool Ignore, bool Lit, bool Wireframe);

IMaterial * CreateMateralReflective(bool Ignore, bool Lit);

IMaterial * CreateMaterial2(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert);