#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>

namespace IL2CPP
{
	namespace ResolverCache
	{
		inline std::unordered_map<std::string, void*> m_IcallCache;
		inline std::unordered_map<std::string, void*> m_ManagedMethodCache;
		inline std::mutex m_Mutex;
	}

	inline std::string BuildManagedMethodCacheKey(const char* m_ClassName, const char* m_MethodName, int m_ArgCount)
	{
		std::string m_Key = m_ClassName ? m_ClassName : "";
		m_Key += "::";
		m_Key += m_MethodName ? m_MethodName : "";
		m_Key += "#argc=";
		m_Key += std::to_string(m_ArgCount);
		return m_Key;
	}

	inline std::string BuildManagedMethodCacheKey(const char* m_ClassName, const char* m_MethodName, std::initializer_list<const char*> m_ParamTypes)
	{
		std::string m_Key = m_ClassName ? m_ClassName : "";
		m_Key += "::";
		m_Key += m_MethodName ? m_MethodName : "";
		m_Key += "#sig=";

		for (const char* m_ParamType : m_ParamTypes)
		{
			m_Key += m_ParamType ? m_ParamType : "<null>";
			m_Key += ';';
		}

		return m_Key;
	}

	inline void ClearResolverCaches()
	{
		std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
		ResolverCache::m_IcallCache.clear();
		ResolverCache::m_ManagedMethodCache.clear();
	}

	inline void* ResolveUnityMethod(const char* m_ClassName, const char* m_MethodName, int m_ArgCount)
	{
		if (!m_ClassName || !m_MethodName)
			return nullptr;

		const std::string m_CacheKey = BuildManagedMethodCacheKey(m_ClassName, m_MethodName, m_ArgCount);
		{
			std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
			auto m_It = ResolverCache::m_ManagedMethodCache.find(m_CacheKey);
			if (m_It != ResolverCache::m_ManagedMethodCache.end())
				return m_It->second;
		}

		Unity::il2cppClass* m_pClass = IL2CPP::Class::Find(m_ClassName);
		if (!m_pClass)
			return nullptr;

		void* m_pMethod = IL2CPP::Class::Utils::GetMethodPointer(m_pClass, m_MethodName, m_ArgCount);
		std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
		ResolverCache::m_ManagedMethodCache.emplace(m_CacheKey, m_pMethod);
		return m_pMethod;
	}

	inline void* ResolveUnityMethod(const char* m_ClassName, const char* m_MethodName, std::initializer_list<const char*> m_ParamTypes)
	{
		if (!m_ClassName || !m_MethodName)
			return nullptr;

		const std::string m_CacheKey = BuildManagedMethodCacheKey(m_ClassName, m_MethodName, m_ParamTypes);
		{
			std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
			auto m_It = ResolverCache::m_ManagedMethodCache.find(m_CacheKey);
			if (m_It != ResolverCache::m_ManagedMethodCache.end())
				return m_It->second;
		}

		Unity::il2cppClass* m_pClass = IL2CPP::Class::Find(m_ClassName);
		if (!m_pClass)
			return nullptr;

		void* m_pMethod = IL2CPP::Class::Utils::GetMethodPointer(m_pClass, m_MethodName, m_ParamTypes);
		std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
		ResolverCache::m_ManagedMethodCache.emplace(m_CacheKey, m_pMethod);
		return m_pMethod;
	}

	inline void* ResolveCall(const char* m_Name)
	{
		if (!Functions.m_ResolveFunction || !m_Name)
			return nullptr;

		return reinterpret_cast<void* (IL2CPP_CALLING_CONVENTION)(const char*)>(Functions.m_ResolveFunction)(m_Name);
	}

	inline void* ResolveCallCached(const char* m_Name)
	{
		if (!m_Name)
			return nullptr;

		{
			std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
			auto m_It = ResolverCache::m_IcallCache.find(m_Name);
			if (m_It != ResolverCache::m_IcallCache.end())
				return m_It->second;
		}

		void* m_pCall = ResolveCall(m_Name);
		std::lock_guard<std::mutex> m_Lock(ResolverCache::m_Mutex);
		ResolverCache::m_IcallCache.emplace(m_Name, m_pCall);
		return m_pCall;
	}

	inline void* ResolveCallAny(std::initializer_list<const char*> m_Names)
	{
		for (const char* m_pName : m_Names)
		{
			if (!m_pName)
				continue;

			if (void* m_pCall = ResolveCallCached(m_pName))
				return m_pCall;
		}

		return nullptr;
	}

	inline void* ResolveUnityMethodOrIcall(const char* m_ClassName, const char* m_MethodName, int m_ArgCount,
		std::initializer_list<const char*> m_IcallCandidates = {})
	{
		if (void* m_pMethod = ResolveUnityMethod(m_ClassName, m_MethodName, m_ArgCount))
			return m_pMethod;

		if (!m_IcallCandidates.size())
			return nullptr;

		return ResolveCallAny(m_IcallCandidates);
	}

	inline void* ResolveUnityMethodOrIcall(const char* m_ClassName, const char* m_MethodName,
		std::initializer_list<const char*> m_ParamTypes,
		std::initializer_list<const char*> m_IcallCandidates = {})
	{
		if (void* m_pMethod = ResolveUnityMethod(m_ClassName, m_MethodName, m_ParamTypes))
			return m_pMethod;

		if (!m_IcallCandidates.size())
			return nullptr;

		return ResolveCallAny(m_IcallCandidates);
	}
}
