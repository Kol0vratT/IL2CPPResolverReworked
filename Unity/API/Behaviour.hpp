#pragma once

namespace Unity
{
	struct BehaviourFunctions_t
	{
		void* m_GetEnabled = nullptr; bool m_GetEnabled_ThisIsPtr = false;
		void* m_SetEnabled = nullptr; bool m_SetEnabled_ThisIsPtr = false;
		void* m_GetIsActiveAndEnabled = nullptr; bool m_GetIsActiveAndEnabled_ThisIsPtr = false;
	};
	inline BehaviourFunctions_t m_BehaviourFunctions;

	class CBehaviour : public CComponent
	{
	public:
		bool GetEnabled()
		{
			if (!this || !m_BehaviourFunctions.m_GetEnabled)
				return false;

			void* selfArg = m_BehaviourFunctions.m_GetEnabled_ThisIsPtr ? this->m_CachedPtr : this->GetManagedObjectPointer();
			if (!selfArg)
				return false;

			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(void*)>(m_BehaviourFunctions.m_GetEnabled)(selfArg);
		}

		void SetEnabled(bool value)
		{
			if (!this || !m_BehaviourFunctions.m_SetEnabled)
				return;

			void* selfArg = m_BehaviourFunctions.m_SetEnabled_ThisIsPtr ? this->m_CachedPtr : this->GetManagedObjectPointer();
			if (!selfArg)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, bool)>(m_BehaviourFunctions.m_SetEnabled)(selfArg, value);
		}

		bool GetIsActiveAndEnabled()
		{
			if (!this || !m_BehaviourFunctions.m_GetIsActiveAndEnabled)
				return false;

			void* selfArg = m_BehaviourFunctions.m_GetIsActiveAndEnabled_ThisIsPtr ? this->m_CachedPtr : this->GetManagedObjectPointer();
			if (!selfArg)
				return false;

			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(void*)>(m_BehaviourFunctions.m_GetIsActiveAndEnabled)(selfArg);
		}
	};

	namespace Behaviour
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_BEHAVIOUR_CLASS);

			auto resolveInstance = [&](void*& outPtr, bool& outThisIsPtr,
				const char* methodName, int argCount,
				const char* injectedMethodName, int injectedArgCount,
				std::initializer_list<const char*> icallObj,
				std::initializer_list<const char*> icallPtr)
			{
				outPtr = nullptr;
				outThisIsPtr = false;

				if (void* p = IL2CPP::ResolveUnityMethod(UNITY_BEHAVIOUR_CLASS, methodName, argCount))
				{
					outPtr = p;
					return;
				}

				for (const char* n : icallObj)
				{
					if (!n)
						continue;

					if (void* p = IL2CPP::ResolveCallCached(n))
					{
						outPtr = p;
						return;
					}
				}

				if (void* p = IL2CPP::ResolveUnityMethod(UNITY_BEHAVIOUR_CLASS, injectedMethodName, injectedArgCount))
				{
					outPtr = p;
					outThisIsPtr = true;
					return;
				}

				if (void* p = IL2CPP::ResolveCallAny(icallPtr))
				{
					outPtr = p;
					outThisIsPtr = true;
					return;
				}
			};

			resolveInstance(m_BehaviourFunctions.m_GetEnabled, m_BehaviourFunctions.m_GetEnabled_ThisIsPtr,
				"get_enabled", 0,
				"get_enabled_Injected", 1,
				{ UNITY_BEHAVIOUR_GETENABLED, IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::get_enabled") },
				{ IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::get_enabled_Injected"),
				  IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::get_enabled_Injected(System.IntPtr)") });

			resolveInstance(m_BehaviourFunctions.m_SetEnabled, m_BehaviourFunctions.m_SetEnabled_ThisIsPtr,
				"set_enabled", 1,
				"set_enabled_Injected", 2,
				{ UNITY_BEHAVIOUR_SETENABLED, IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::set_enabled") },
				{ IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::set_enabled_Injected"),
				  IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::set_enabled_Injected(System.IntPtr,System.Boolean)") });

			resolveInstance(m_BehaviourFunctions.m_GetIsActiveAndEnabled, m_BehaviourFunctions.m_GetIsActiveAndEnabled_ThisIsPtr,
				"get_isActiveAndEnabled", 0,
				"get_isActiveAndEnabled_Injected", 1,
				{ UNITY_BEHAVIOUR_GETISACTIVEANDENABLED, IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::get_isActiveAndEnabled") },
				{ IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::get_isActiveAndEnabled_Injected"),
				  IL2CPP_RStr(UNITY_BEHAVIOUR_CLASS"::get_isActiveAndEnabled_Injected(System.IntPtr)") });
		}
	}
}
