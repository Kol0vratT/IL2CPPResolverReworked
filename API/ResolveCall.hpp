#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>

namespace IL2CPP
{
	// Resolve a managed method's native code pointer by name/argcount.
	// This is often more stable across Unity versions than icall name strings.
	inline void* ResolveUnityMethod(const char* m_ClassName, const char* m_MethodName, int m_ArgCount)
	{
		if (!m_ClassName || !m_MethodName)
			return nullptr;

		Unity::il2cppClass* klass = IL2CPP::Class::Find(m_ClassName);
		if (!klass)
			return nullptr;

		return IL2CPP::Class::Utils::GetMethodPointer(klass, m_MethodName, m_ArgCount);
	}

	// Thin wrapper around il2cpp_resolve_icall
	inline void* ResolveCall(const char* m_Name)
	{
		if (!Functions.m_ResolveFunction || !m_Name)
			return nullptr;

		return reinterpret_cast<void* (IL2CPP_CALLING_CONVENTION)(const char*)>(Functions.m_ResolveFunction)(m_Name);
	}

	// Cached icall resolver (safe for header-only usage)
	inline void* ResolveCallCached(const char* m_Name)
	{
		static std::unordered_map<std::string, void*> s_Cache;
		auto it = s_Cache.find(m_Name);
		if (it != s_Cache.end())
			return it->second;

		void* p = ResolveCall(m_Name);
		s_Cache.emplace(m_Name, p);
		return p;
	}

	// Try multiple icall names (Unity 6 often requires *_Injected or a fully qualified signature)
	inline void* ResolveCallAny(std::initializer_list<const char*> m_Names)
	{
		for (const char* n : m_Names)
		{
			if (!n)
				continue;
			if (void* p = ResolveCallCached(n))
				return p;
		}
		return nullptr;
	}

	// Prefer resolving managed method pointers (more stable across Unity versions)
	// Falls back to icall resolution if needed.
	inline void* ResolveUnityMethodOrIcall(const char* m_ClassName, const char* m_MethodName, int m_ArgCount,
		std::initializer_list<const char*> m_IcallCandidates = {})
	{
		// Resolve by managed metadata first
		if (void* mp = ResolveUnityMethod(m_ClassName, m_MethodName, m_ArgCount))
			return mp;

		// Fallback: icall candidates (old name, injected name, fully qualified signature, etc.)
		if (m_IcallCandidates.size() > 0)
			return ResolveCallAny(m_IcallCandidates);

		return nullptr;
	}
}
