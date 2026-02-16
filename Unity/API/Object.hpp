#pragma once

namespace Unity
{
	struct ObjectFunctions_t
	{
		void* m_Destroy = nullptr;
		void* m_FindObjectsOfType = nullptr;   // FindObjectsOfType(Type, bool)
		void* m_FindObjectsByType2 = nullptr;  // FindObjectsByType(Type, FindObjectsSortMode)
		void* m_FindObjectsByType3 = nullptr;  // FindObjectsByType(Type, FindObjectsSortMode, FindObjectsInactive)
		void* m_GetName = nullptr;
	};
	ObjectFunctions_t m_ObjectFunctions;

	class CObject : public IL2CPP::CClass
	{
	public:
		void Destroy(float fTimeDelay = 0.f)
		{
			if (!this || !m_ObjectFunctions.m_Destroy)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, float)>(m_ObjectFunctions.m_Destroy)(this, fTimeDelay);
		}

		System_String* GetName()
		{
			if (!this || !m_ObjectFunctions.m_GetName)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)(void*)>(m_ObjectFunctions.m_GetName)(this);
		}
	};

	namespace Object
	{
		void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_OBJECT_CLASS);

			// Prefer managed method pointers (stable across Unity versions)
			m_ObjectFunctions.m_Destroy = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_OBJECT_CLASS, "Destroy", 2, // static Destroy(Object, float)
				{ UNITY_OBJECT_DESTROY, IL2CPP_RStr(UNITY_OBJECT_CLASS"::Destroy_Injected") });

			// FindObjectsOfType(Type, bool) (legacy but still present in many games)
			m_ObjectFunctions.m_FindObjectsOfType = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_OBJECT_CLASS, "FindObjectsOfType", 2,
				{ UNITY_OBJECT_FINDOBJECTSOFTYPE, IL2CPP_RStr(UNITY_OBJECT_CLASS"::FindObjectsOfType_Injected") });

			// Unity 6 / 6000.x preferred APIs
			m_ObjectFunctions.m_FindObjectsByType2 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_OBJECT_CLASS, "FindObjectsByType", 2,
				{
					IL2CPP_RStr(UNITY_OBJECT_CLASS"::FindObjectsByType(System.Type,UnityEngine.FindObjectsSortMode)"),
					IL2CPP_RStr(UNITY_OBJECT_CLASS"::FindObjectsByType(System.Type,int)"),
				});

			m_ObjectFunctions.m_FindObjectsByType3 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_OBJECT_CLASS, "FindObjectsByType", 3,
				{
					IL2CPP_RStr(UNITY_OBJECT_CLASS"::FindObjectsByType(System.Type,UnityEngine.FindObjectsSortMode,UnityEngine.FindObjectsInactive)"),
					IL2CPP_RStr(UNITY_OBJECT_CLASS"::FindObjectsByType(System.Type,int,int)"),
				});

			m_ObjectFunctions.m_GetName = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_OBJECT_CLASS, "GetName", 1,
				{ UNITY_OBJECT_GETNAME, IL2CPP_RStr(UNITY_OBJECT_CLASS"::GetName_Injected") });
		}

		static il2cppObject* New(il2cppClass* m_pClass)
		{
			if (!IL2CPP::Functions.m_pObjectNew || !m_pClass)
				return nullptr;

			return reinterpret_cast<Unity::il2cppObject * (UNITY_CALLING_CONVENTION)(void*)>(IL2CPP::Functions.m_pObjectNew)(m_pClass);
		}

		template<typename T>
		static il2cppArray<T*>* FindObjectsOfType(il2cppObject* m_pSystemType, bool m_bIncludeInactive = false)
		{
			if (!m_pSystemType)
				return nullptr;

			// Preferred: legacy signature FindObjectsOfType(Type, bool)
			if (m_ObjectFunctions.m_FindObjectsOfType)
				return reinterpret_cast<Unity::il2cppArray<T*>*(UNITY_CALLING_CONVENTION)(void*, bool)>(m_ObjectFunctions.m_FindObjectsOfType)(m_pSystemType, m_bIncludeInactive);

			// Unity 6+ fallback: FindObjectsByType(Type, FindObjectsSortMode, FindObjectsInactive)
			// We pass sortMode = 0 (None) and includeInactive mapped to 1/0.
			if (m_ObjectFunctions.m_FindObjectsByType3)
				return reinterpret_cast<Unity::il2cppArray<T*>*(UNITY_CALLING_CONVENTION)(void*, int, int)>(m_ObjectFunctions.m_FindObjectsByType3)(m_pSystemType, 0, m_bIncludeInactive ? 1 : 0);

			// Unity 6+ fallback: FindObjectsByType(Type, FindObjectsSortMode)
			if (m_ObjectFunctions.m_FindObjectsByType2)
				return reinterpret_cast<Unity::il2cppArray<T*>*(UNITY_CALLING_CONVENTION)(void*, int)>(m_ObjectFunctions.m_FindObjectsByType2)(m_pSystemType, 0);

			return nullptr;
		}

		template<typename T>
		static il2cppArray<T*>* FindObjectsOfType(const char* m_pSystemTypeName, bool m_bIncludeInactive = false)
		{
			if (!m_pSystemTypeName)
				return nullptr;

			il2cppClass* m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
			if (!m_pClass) return nullptr;

			return FindObjectsOfType<T>(IL2CPP::Class::GetSystemType(m_pClass), m_bIncludeInactive);
		}

		template<typename T>
		static T* FindObjectOfType(il2cppObject* m_pSystemType, bool m_bIncludeInactive = false)
		{
			il2cppArray<T*>* m_pArray = FindObjectsOfType<T>(m_pSystemType, m_bIncludeInactive);
			if (!m_pArray || m_pArray->m_uMaxLength == 0U) return nullptr;

			return m_pArray->m_pValues[0];
		}

		template<typename T>
		static T* FindObjectOfType(const char* m_pSystemTypeName, bool m_bIncludeInactive = false)
		{
			if (!m_pSystemTypeName)
				return nullptr;

			il2cppClass* m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
			if (!m_pClass) return nullptr;

			return FindObjectOfType<T>(IL2CPP::Class::GetSystemType(m_pClass), m_bIncludeInactive);
		}
	}
}
