#include "Chams.h"
#include "SDK.h"
#include "Interfaces.h"
#include <sstream>
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

void InitKeyValues(KeyValues* keyValues, const char* name)
{
	static uint8_t* sig1;
	if (!sig1)
	{
		sig1 = U::pattern_scan(GetModuleHandleA(U::ClientModule()), "68 ? ? ? ? 8B C8 E8 ? ? ? ? 89 45 FC EB 07 C7 45 ? ? ? ? ? 8B 03 56");
		sig1 += 7;
		sig1 = sig1 + *reinterpret_cast<PDWORD_PTR>(sig1 + 1) + 5;
	}

	static auto function = (void(__thiscall*)(KeyValues*, const char*))sig1;
	function(keyValues, name);
}

void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer)
{
	static uint8_t* offset;
	if (!offset) offset = U::pattern_scan(GetModuleHandleA(U::ClientModule()), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
	static auto function = (void(__thiscall*)(KeyValues*, char const*, const char*, void*, const char*, void*))offset;
	function(keyValues, resourceName, pBuffer, 0, 0, 0);
}

IMaterial* CreateMaterial(bool Ignore, bool Lit, bool Wireframe)
{
	static int created = 0;
	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\"  \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\"  \"0\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};
	char* baseType = (Lit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	char name[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (Ignore) ? 1 : 0, (Wireframe) ? 1 : 0);
	sprintf_s(name, sizeof(name), "#Aimpulse_Chams_%i.vmt", created);
	++created;

	KeyValues* keyValues = new KeyValues(name);
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);
	IMaterial* createdMaterial = g_MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();
	return createdMaterial;
}

IMaterial* CreateMateralReflective(bool Ignore, bool Lit)
{
	static int created = 0;
	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\"  \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\"  \"0\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};
	char* baseType = (Lit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	char name[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (Ignore) ? 1 : 0, 0);
	sprintf_s(name, sizeof(name), "#Aimpulse_Chams_%i.vmt", created);
	++created;

	KeyValues* keyValues = new KeyValues(name);
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);
	IMaterial* createdMaterial = g_MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();
	return createdMaterial;
}


IMaterial* CreateMaterial2(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert)
{
	static int number = 0;
	std::stringstream materialData;
	materialData << "\"" + type + "\"\n"
		"{\n"
		"\t\"$basetexture\" \"" + texture + "\"\n"
		"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
		"\t\"$envmap\" \"" + "" + "\"\n"
		"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
		"\t\"$model\" \"" + std::to_string(model) + "\"\n"
		"\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
		"\t\"$selfillum\" \"" + "1" + "\"\n"
		"\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
		"\t\"$znearer\" \"" + "0" + "\"\n"
		"\t\"$flat\" \"" + "1" + "\"\n"
		"}\n" << std::flush;

	std::string materialName = "material_" + std::to_string(number) + ".vmt";
	KeyValues* keyValues = new KeyValues(materialName.c_str());
	number++;
	InitKeyValues(keyValues, type.c_str());
	LoadFromBuffer(keyValues, materialName.c_str(), materialData.str().c_str());
	return g_MaterialSystem->CreateMaterial(materialName.c_str(), keyValues);
}


void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			(float)color.r(),
			(float)color.g(),
			(float)color.b()
		};

		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;


		float alpha = (float)color.a();

		g_RenderView->SetBlend(1.0f);
		g_RenderView->SetColorModulation(temp);
	}

	if (forceMaterial)
		g_ModelRender->ForcedMaterialOverride(material);
	else
		g_ModelRender->ForcedMaterialOverride(NULL);

}

void *KeyValues::operator new(size_t iAllocSize)
{
	static PVOID pKeyValuesSystem;
	if (!pKeyValuesSystem)
	{
		typedef PVOID(__cdecl* oKeyValuesSystem)();
		oKeyValuesSystem KeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");

		pKeyValuesSystem = KeyValuesSystem();
	}

	typedef PVOID(__thiscall* oAllocKeyValuesMemory)(PVOID, size_t);
	return  call_vfunc<oAllocKeyValuesMemory>(pKeyValuesSystem, 1)(pKeyValuesSystem, iAllocSize);
}

void KeyValues::operator delete(void * pMem)
{
	static PVOID pKeyValuesSystem;
	if (!pKeyValuesSystem)
	{
		typedef PVOID(__cdecl* oKeyValuesSystem)();
		oKeyValuesSystem KeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");

		pKeyValuesSystem = KeyValuesSystem();
	}

	typedef void(__thiscall* oFreeKeyValuesMemory)(PVOID, PVOID);
	call_vfunc<oFreeKeyValuesMemory>(pKeyValuesSystem, 2)(pKeyValuesSystem, pMem);
}