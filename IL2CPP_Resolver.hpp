#pragma once

// Default Headers
#include <cstdint>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <Windows.h>

// -----------------------------------------------------------------------------
// Unity version/layout selection
//
// Unity 2022.3.8f1+ (including Unity 2023.x/2024.x and Unity 6 / 6000.x) changed
// multiple IL2CPP structure layouts (most notably Il2CppType / MethodInfo params).
//
// This project is primarily used on modern Unity games; therefore we default to
// the new layout unless the user explicitly opts into the legacy layout.
//
// If you target < 2022.3.8f1, define UNITY_VERSION_PRE_2022_3_8F1 BEFORE including
// IL2CPP_Resolver.hpp.
// -----------------------------------------------------------------------------
//#if !defined(UNITY_VERSION_2022_3_8F1) && !defined(UNITY_VERSION_PRE_2022_3_8F1)
//#define UNITY_VERSION_2022_3_8F1
//#endif

// IL2CPP Defines

// Disable Asserts
//#define IL2CPP_ASSERT(x) assert(0)
#ifndef IL2CPP_ASSERT
#include <cassert>
#define IL2CPP_ASSERT(x) assert(x)
#endif

#ifndef IL2CPP_RStr 
	// If you wanna forward to some string encryption just define before including this file.
#define IL2CPP_RStr(x) x
#endif

#ifndef IL2CPP_MAIN_MODULE
	// If the game for some reason uses diff module name just define own one before including this file.
#define IL2CPP_MAIN_MODULE IL2CPP_RStr("GameAssembly.dll")
#endif

#include "Defines.hpp"

// IL2CPP Headers
#include "Data.hpp"

// Unity Headers
#include "Unity/Obfuscators.hpp"
#include "Unity/Defines.hpp"
#include "Unity/Structures/il2cpp.hpp"
#include "Unity/Structures/il2cppArray.hpp"
#include "Unity/Structures/il2cppDictionary.hpp"
#include "Unity/Structures/Engine.hpp"
#include "Unity/Structures/System_String.hpp"

// IL2CPP Utils
#include "Utils/Hash.hpp"
#include "Utils/VTable.hpp"

// IL2CPP API Headers
#include "API/Domain.hpp"
#include "API/Class.hpp"
#include "API/ResolveCall.hpp"
#include "API/String.hpp"
#include "API/Thread.hpp"

// IL2CPP Headers before Unity API
#include "SystemTypeCache.hpp"

// Unity Class APIs - So they're accessible everywhere
namespace Unity
{
	class CCamera;
	class CComponent;
	class CGameObject;
	class CLayerMask;
	class CObject;
	class CRigidbody;
	class CTransform;
}

// Unity API
#include "Unity/API/Object.hpp"
#include "Unity/API/GameObject.hpp"
#include "Unity/API/Camera.hpp"
#include "Unity/API/Component.hpp"
#include "Unity/API/LayerMask.hpp"
#include "Unity/API/Rigidbody.hpp"
#include "Unity/API/Transform.hpp"
#include "Unity/API/RenderSettings.hpp"
#include "Unity/API/Time.hpp"
#include "Unity/API/Application.hpp"
#include "Unity/API/SceneManager.hpp"
#include "Unity/API/Debug.hpp"
#include "Unity/API/Input.hpp"
#include "Unity/API/Screen.hpp"
#include "Unity/API/Cursor.hpp"

// IL2CPP Headers after Unity API
#include "Utils/Helper.hpp"
#include "API/Callback.hpp"

namespace IL2CPP
{
	namespace UnityAPI
	{
		using ExportResolverCallback_t = void* (*)(HMODULE, const char*);

		enum m_eExportObfuscationType
		{
			None = 0,
			ROT = 1,
			MAX = 2,
		};
		m_eExportObfuscationType m_ExportObfuscation = m_eExportObfuscationType::None;
		ExportResolverCallback_t m_CustomExportResolver = nullptr;
		bool m_EnableHeuristicExportResolution = false;
		std::string m_LastInitError;

		struct ExportSymbol_t
		{
			std::string m_Name;
			void* m_Address = nullptr;
			uint16_t m_Ordinal = 0;
		};
		std::vector<ExportSymbol_t> m_ExportCache;
		bool m_ExportCacheBuilt = false;

		void SetCustomExportResolver(ExportResolverCallback_t m_Callback)
		{
			m_CustomExportResolver = m_Callback;
		}

		const char* GetLastInitError()
		{
			return m_LastInitError.c_str();
		}

		void SetHeuristicExportResolution(bool m_Enable)
		{
			m_EnableHeuristicExportResolution = m_Enable;
		}

		std::string NormalizeExportName(const char* m_Name)
		{
			if (!m_Name)
				return {};

			std::string m_Result;
			for (const char* p = m_Name; *p; ++p)
			{
				unsigned char c = static_cast<unsigned char>(*p);
				if (std::isalnum(c))
					m_Result.push_back(static_cast<char>(std::tolower(c)));
			}

			return m_Result;
		}

		void SplitTokens(const std::string& m_String, std::vector<std::string>* m_Tokens)
		{
			m_Tokens->clear();
			if (m_String.empty())
				return;

			size_t m_Start = 0;
			while (m_Start < m_String.size())
			{
				size_t m_End = m_String.find('_', m_Start);
				if (m_End == std::string::npos)
					m_End = m_String.size();

				if (m_End > m_Start)
					m_Tokens->emplace_back(m_String.substr(m_Start, m_End - m_Start));

				m_Start = m_End + 1;
			}
		}

		bool BuildExportCache()
		{
			if (m_ExportCacheBuilt)
				return true;

			m_ExportCacheBuilt = true;
			m_ExportCache.clear();

			if (!Globals.m_GameAssembly)
				return false;

			auto* m_Base = reinterpret_cast<uint8_t*>(Globals.m_GameAssembly);
			auto* m_Dos = reinterpret_cast<IMAGE_DOS_HEADER*>(m_Base);
			if (!m_Dos || m_Dos->e_magic != IMAGE_DOS_SIGNATURE)
				return false;

			auto* m_Nt = reinterpret_cast<IMAGE_NT_HEADERS*>(m_Base + m_Dos->e_lfanew);
			if (!m_Nt || m_Nt->Signature != IMAGE_NT_SIGNATURE)
				return false;

			const IMAGE_DATA_DIRECTORY& m_ExportDirectory =
				m_Nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
			if (!m_ExportDirectory.VirtualAddress || !m_ExportDirectory.Size)
				return false;

			auto* m_Exports = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(m_Base + m_ExportDirectory.VirtualAddress);
			auto* m_Functions = reinterpret_cast<uint32_t*>(m_Base + m_Exports->AddressOfFunctions);
			auto* m_Names = reinterpret_cast<uint32_t*>(m_Base + m_Exports->AddressOfNames);
			auto* m_NameOrdinals = reinterpret_cast<uint16_t*>(m_Base + m_Exports->AddressOfNameOrdinals);

			for (uint32_t i = 0; i < m_Exports->NumberOfNames; ++i)
			{
				const char* m_Name = reinterpret_cast<const char*>(m_Base + m_Names[i]);
				uint16_t m_NameOrdinal = m_NameOrdinals[i];
				uint32_t m_Rva = m_Functions[m_NameOrdinal];
				void* m_Address = reinterpret_cast<void*>(m_Base + m_Rva);

				ExportSymbol_t m_Symbol;
				m_Symbol.m_Name = m_Name ? m_Name : "";
				m_Symbol.m_Address = m_Address;
				m_Symbol.m_Ordinal = static_cast<uint16_t>(m_Exports->Base + m_NameOrdinal);
				m_ExportCache.emplace_back(m_Symbol);
			}

			return !m_ExportCache.empty();
		}

		void* ResolveExportByHeuristic(const char* m_CanonicalName)
		{
			if (!m_CanonicalName || !BuildExportCache())
				return nullptr;

			std::string m_NormalizedCanonical = NormalizeExportName(m_CanonicalName);
			if (m_NormalizedCanonical.empty())
				return nullptr;

			std::vector<std::string> m_CanonicalTokens;
			SplitTokens(m_CanonicalName, &m_CanonicalTokens);
			if (!m_CanonicalTokens.empty() && m_CanonicalTokens[0] == "il2cpp")
				m_CanonicalTokens.erase(m_CanonicalTokens.begin());

			void* m_BestAddress = nullptr;
			int m_BestScore = -1;
			for (const auto& m_Symbol : m_ExportCache)
			{
				std::string m_NormalizedSymbol = NormalizeExportName(m_Symbol.m_Name.c_str());
				if (m_NormalizedSymbol == m_NormalizedCanonical)
					return m_Symbol.m_Address;

				// Conservative fuzzy fallback: only if candidate still contains il2cpp and most tokens match.
				if (m_Symbol.m_Name.find("il2cpp") == std::string::npos)
					continue;

				int m_Score = 0;
				for (const std::string& m_Token : m_CanonicalTokens)
				{
					if (m_Token.empty())
						continue;

					if (m_Symbol.m_Name.find(m_Token) != std::string::npos)
						++m_Score;
				}

				if (!m_CanonicalTokens.empty() && m_Score >= static_cast<int>(m_CanonicalTokens.size()) - 1)
				{
					if (m_Score > m_BestScore)
					{
						m_BestScore = m_Score;
						m_BestAddress = m_Symbol.m_Address;
					}
				}
			}

			return m_BestAddress;
		}

		int m_ROTObfuscationValue = -1;
		void* ResolveExport(const char* m_Name)
		{
			if (!m_Name)
				return nullptr;

			switch (m_ExportObfuscation)
			{
			case m_eExportObfuscationType::ROT:
			{
				if (m_ROTObfuscationValue == -1) // Bruteforce
				{
					for (int i = 1; 26 > i; ++i)
					{
						void* m_Return = GetProcAddress(Globals.m_GameAssembly, &Unity::Obfuscators::ROT_String(m_Name, i)[0]);
						if (m_Return)
						{
							m_ROTObfuscationValue = i;
							return m_Return;
						}
					}

					return nullptr;
				}

				return GetProcAddress(Globals.m_GameAssembly, &Unity::Obfuscators::ROT_String(m_Name, m_ROTObfuscationValue)[0]);
			}
			default: return GetProcAddress(Globals.m_GameAssembly, m_Name);
			}

			return nullptr;
		}

		void* ResolveExportAny(std::initializer_list<const char*> m_Names, bool m_AllowHeuristic)
		{
			for (const char* m_Name : m_Names)
			{
				if (!m_Name)
					continue;

				if (void* m_Address = ResolveExport(m_Name))
					return m_Address;
			}

			// External/custom resolver hook for heavily modified loaders or obfuscated exports.
			if (m_CustomExportResolver)
			{
				for (const char* m_Name : m_Names)
				{
					if (!m_Name)
						continue;

					if (void* m_Address = m_CustomExportResolver(Globals.m_GameAssembly, m_Name))
						return m_Address;
				}
			}

			if (m_AllowHeuristic && m_EnableHeuristicExportResolution)
			{
				// Last chance: named export heuristics. Disabled by default because false positives are dangerous.
				for (const char* m_Name : m_Names)
				{
					if (!m_Name)
						continue;

					if (void* m_Address = ResolveExportByHeuristic(m_Name))
						return m_Address;
				}
			}

			return nullptr;
		}

		bool Initialize()
		{
			// Clear previously resolved state for safe re-init attempts.
			Functions = {};
			m_ExportCache.clear();
			m_ExportCacheBuilt = false;
			m_ROTObfuscationValue = -1;
			m_LastInitError.clear();

			bool m_InitExportResolved = false;
			for (int i = 0; m_eExportObfuscationType::MAX > i; ++i)
			{
				m_ExportObfuscation = static_cast<m_eExportObfuscationType>(i);
				if (ResolveExport(IL2CPP_INIT_EXPORT))
				{
					m_InitExportResolved = true;
					break;
				}
			}

			if (!m_InitExportResolved)
			{
				m_LastInitError = "Failed to resolve il2cpp_init in selected module";
				return false;
			}

			auto resolveRequired = [&](void** m_Address, const char* m_DebugName, std::initializer_list<const char*> m_Names)
				{
					*m_Address = ResolveExportAny(m_Names, false);
					if (!*m_Address)
						m_LastInitError = std::string("Missing required export: ") + (m_DebugName ? m_DebugName : "<unknown>");
					return (*m_Address != nullptr);
				};

			auto resolveOptional = [&](void** m_Address, std::initializer_list<const char*> m_Names)
				{
					*m_Address = ResolveExportAny(m_Names, false);
				};

			// Required core exports
			if (!resolveRequired(&Functions.m_ClassFromName, "il2cpp_class_from_name", { IL2CPP_CLASS_FROM_NAME_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_ClassGetFields, "il2cpp_class_get_fields", { IL2CPP_CLASS_GET_FIELDS })) return false;
			if (!resolveRequired(&Functions.m_ClassGetFieldFromName, "il2cpp_class_get_field_from_name", { IL2CPP_CLASS_GET_FIELD_FROM_NAME_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_ClassGetMethods, "il2cpp_class_get_methods", { IL2CPP_CLASS_GET_METHODS })) return false;
			if (!resolveRequired(&Functions.m_ClassGetMethodFromName, "il2cpp_class_get_method_from_name", { IL2CPP_CLASS_GET_METHOD_FROM_NAME_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_ClassGetPropertyFromName, "il2cpp_class_get_property_from_name", { IL2CPP_CLASS_GET_PROPERTY_FROM_NAME_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_ClassGetType, "il2cpp_class_get_type", { IL2CPP_CLASS_GET_TYPE_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_DomainGet, "il2cpp_domain_get", { IL2CPP_DOMAIN_GET_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_DomainGetAssemblies, "il2cpp_domain_get_assemblies", { IL2CPP_DOMAIN_GET_ASSEMBLIES_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_StringNew, "il2cpp_string_new", { IL2CPP_STRING_NEW_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_ThreadAttach, "il2cpp_thread_attach", { IL2CPP_THREAD_ATTACH_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_ThreadDetach, "il2cpp_thread_detach", { IL2CPP_THREAD_DETACH_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_TypeGetObject, "il2cpp_type_get_object", { IL2CPP_TYPE_GET_OBJECT_EXPORT })) return false;
			if (!resolveRequired(&Functions.m_pObjectNew, "il2cpp_object_new", { IL2CPP_OBJECT_NEW })) return false;

			// Optional / version-specific aliases
			resolveOptional(&Functions.m_ImageGetClass, { IL2CPP_IMAGE_GET_CLASS_EXPORT });
			resolveOptional(&Functions.m_Free, { IL2CPP_FREE_EXPORT });
			resolveOptional(&Functions.m_ImageGetClassCount, { IL2CPP_IMAGE_GET_CLASS_COUNT_EXPORT });
			resolveOptional(&Functions.m_ResolveFunction, { IL2CPP_RESOLVE_FUNC_EXPORT, IL2CPP_RStr("il2cpp_codegen_resolve_icall") });
			resolveOptional(&Functions.m_MethodGetParamName, { IL2CPP_METHOD_GET_PARAM_NAME });
			resolveOptional(&Functions.m_MethodGetParam, { IL2CPP_METHOD_GET_PARAM });
			resolveOptional(&Functions.m_ClassFromIl2cppType, { IL2CPP_CLASS_FROM_IL2CPP_TYPE, IL2CPP_RStr("il2cpp_class_from_type") });
			resolveOptional(&Functions.m_FieldStaticGetValue, { IL2CPP_FIELD_STATIC_GET_VALUE });
			resolveOptional(&Functions.m_FieldStaticSetValue, { IL2CPP_FIELD_STATIC_SET_VALUE });

			// Unity APIs
			Unity::Camera::Initialize();
			Unity::Component::Initialize();
			Unity::GameObject::Initialize();
			Unity::LayerMask::Initialize();
			Unity::Object::Initialize();
			Unity::RigidBody::Initialize();
			Unity::Transform::Initialize();
			Unity::RenderSettings::Initialize();
			Unity::Time::Initialize();
			Unity::Application::Initialize();
			Unity::SceneManager::Initialize();
			Unity::Debug::Initialize();
			Unity::Input::Initialize();
			Unity::Screen::Initialize();
			Unity::Cursor::Initialize();

			// Caches
			IL2CPP::SystemTypeCache::Initializer::PreCache();

			return true;
		}
	}

	// Optional hook: provide your own resolver for protected/obfuscated builds.
	// Called when default export name resolution fails.
	inline void SetCustomExportResolver(UnityAPI::ExportResolverCallback_t m_Callback)
	{
		UnityAPI::SetCustomExportResolver(m_Callback);
	}

	// Disabled by default. Enable only if you're sure your target rewrites export names
	// and you can tolerate occasional false positives.
	inline void SetHeuristicExportResolution(bool m_Enable)
	{
		UnityAPI::SetHeuristicExportResolution(m_Enable);
	}

	/*
	*	You need to call this, before using any IL2CPP/Unity Functions!
	*	Args:
	*		m_WaitForModule - Will wait for main module if you're loading your dll earlier than the main module.
	*		m_MaxSecondsWait - Max seconds it will wait for main module to load otherwise will return false to prevent infinite loop.
	*/
	bool Initialize(bool m_WaitForModule = false, int m_MaxSecondsWait = 60)
	{
		auto resolveModule = []() -> HMODULE
			{
				// Primary target.
				if (HMODULE m_Module = GetModuleHandleA(IL2CPP_MAIN_MODULE))
					return m_Module;

				// Common alternatives across Unity/loader setups.
				static const char* m_CommonModules[] =
				{
					"GameAssembly.dll",
					"UserAssembly.dll",
					"libil2cpp.dll",
					"UnityPlayer.dll",
				};

				for (const char* m_Name : m_CommonModules)
				{
					if (!m_Name)
						continue;
					if (_stricmp(m_Name, IL2CPP_MAIN_MODULE) == 0)
						continue;

					if (HMODULE m_Module = GetModuleHandleA(m_Name))
						return m_Module;
				}

				return nullptr;
			};

		Globals.m_GameAssembly = resolveModule();

		if (m_WaitForModule)
		{
			int m_SecondsWaited = 0;
			while (!Globals.m_GameAssembly)
			{
				if (m_SecondsWaited >= m_MaxSecondsWait)
					return false;

				Globals.m_GameAssembly = resolveModule();
				++m_SecondsWaited;
				Sleep(1000);
			}
		}

		if (!UnityAPI::Initialize())
			return false;

		return true;
	}
}
