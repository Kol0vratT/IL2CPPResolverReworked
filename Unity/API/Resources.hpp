#pragma once

namespace Unity
{
	struct ResourcesFunctions_t
	{
		void* m_Load1 = nullptr;
		void* m_Load2 = nullptr;
		void* m_FindObjectsOfTypeAll = nullptr;
		void* m_UnloadUnusedAssets = nullptr;
	};
	inline ResourcesFunctions_t m_ResourcesFunctions;

	namespace Resources
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_RESOURCES_CLASS);
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_ASYNCOP_CLASS);

			m_ResourcesFunctions.m_Load1 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RESOURCES_CLASS, "Load",
				{ "System.String" },
				{ UNITY_RESOURCES_LOAD1 });

			m_ResourcesFunctions.m_Load2 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RESOURCES_CLASS, "Load",
				{ "System.String", "System.Type" },
				{ UNITY_RESOURCES_LOAD2 });

			m_ResourcesFunctions.m_FindObjectsOfTypeAll = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RESOURCES_CLASS, "FindObjectsOfTypeAll",
				{ "System.Type" },
				{ UNITY_RESOURCES_FINDOBJECTSOFTYPEALL });

			m_ResourcesFunctions.m_UnloadUnusedAssets = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RESOURCES_CLASS, "UnloadUnusedAssets", 0,
				{ UNITY_RESOURCES_UNLOADUNUSEDASSETS });
		}

		inline CObject* Load(const char* path)
		{
			if (!m_ResourcesFunctions.m_Load1 || !path)
				return nullptr;

			System_String* sPath = IL2CPP::String::New(path);
			if (!sPath)
				return nullptr;

			return reinterpret_cast<CObject * (UNITY_CALLING_CONVENTION)(System_String*)>(m_ResourcesFunctions.m_Load1)(sPath);
		}

		inline CObject* Load(const char* path, il2cppObject* systemType)
		{
			if (!m_ResourcesFunctions.m_Load2 || !path || !systemType)
				return Load(path);

			System_String* sPath = IL2CPP::String::New(path);
			if (!sPath)
				return nullptr;

			return reinterpret_cast<CObject * (UNITY_CALLING_CONVENTION)(System_String*, il2cppObject*)>(m_ResourcesFunctions.m_Load2)(sPath, systemType);
		}

		inline CObject* Load(const char* path, const char* systemTypeName)
		{
			if (!systemTypeName)
				return Load(path);

			il2cppClass* m_pClass = IL2CPP::Class::Find(systemTypeName);
			if (!m_pClass)
				return Load(path);

			return Load(path, IL2CPP::Class::GetSystemType(m_pClass));
		}

		template<typename T>
		inline il2cppArray<T*>* FindObjectsOfTypeAll(il2cppObject* systemType)
		{
			if (!m_ResourcesFunctions.m_FindObjectsOfTypeAll || !systemType)
				return nullptr;

			return reinterpret_cast<il2cppArray<T*> * (UNITY_CALLING_CONVENTION)(il2cppObject*)>(m_ResourcesFunctions.m_FindObjectsOfTypeAll)(systemType);
		}

		template<typename T>
		inline il2cppArray<T*>* FindObjectsOfTypeAll(const char* systemTypeName)
		{
			if (!systemTypeName)
				return nullptr;

			il2cppClass* m_pClass = IL2CPP::Class::Find(systemTypeName);
			if (!m_pClass)
				return nullptr;

			return FindObjectsOfTypeAll<T>(IL2CPP::Class::GetSystemType(m_pClass));
		}

		inline il2cppObject* UnloadUnusedAssets()
		{
			if (!m_ResourcesFunctions.m_UnloadUnusedAssets)
				return nullptr;

			return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)()>(m_ResourcesFunctions.m_UnloadUnusedAssets)();
		}
	}
}
