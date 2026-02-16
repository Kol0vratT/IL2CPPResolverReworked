#pragma once

namespace Unity
{
	struct ComponentFunctions_t
	{
		void* m_GetGameObject = nullptr; bool m_GetGameObject_ThisIsPtr = false;
		void* m_GetTransform = nullptr; bool m_GetTransform_ThisIsPtr = false;
	};
	inline ComponentFunctions_t m_ComponentFunctions;

	class CComponent : public CObject
	{
	public:
		CGameObject* GetGameObject()
		{
			if (!this || !m_ComponentFunctions.m_GetGameObject)
				return nullptr;

			void* selfArg = m_ComponentFunctions.m_GetGameObject_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CGameObject * (UNITY_CALLING_CONVENTION)(void*)>(m_ComponentFunctions.m_GetGameObject)(selfArg);
		}

		CTransform* GetTransform()
		{
			if (!this || !m_ComponentFunctions.m_GetTransform)
				return nullptr;

			void* selfArg = m_ComponentFunctions.m_GetTransform_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*)>(m_ComponentFunctions.m_GetTransform)(selfArg);
		}
	};

	namespace Component
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_COMPONENT_CLASS);

			auto resolveInstance = [&](void*& outPtr, bool& outThisIsPtr,
				const char* methodName, int argCount,
				const char* injectedMethodName, int injectedArgCount,
				std::initializer_list<const char*> icallObj,
				std::initializer_list<const char*> icallPtr)
				{
					outPtr = nullptr;
					outThisIsPtr = false;

					// 1) Prefer managed wrapper (expects UnityEngine.Component 'this')
					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_COMPONENT_CLASS, methodName, argCount))
					{
						outPtr = p;
						return;
					}

					// 2) Try old icall (object 'this')
					for (const char* n : icallObj)
					{
						if (!n) continue;
						if (void* p = IL2CPP::ResolveCallCached(n))
						{
							outPtr = p;
							outThisIsPtr = false;
							return;
						}
					}

					// 3) Try injected managed wrapper (IntPtr 'this')
					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_COMPONENT_CLASS, injectedMethodName, injectedArgCount))
					{
						outPtr = p;
						outThisIsPtr = true;
						return;
					}

					// 4) Try injected icall candidates
					if (void* p = IL2CPP::ResolveCallAny(icallPtr))
					{
						outPtr = p;
						outThisIsPtr = true;
						return;
					}
				};

			resolveInstance(m_ComponentFunctions.m_GetGameObject, m_ComponentFunctions.m_GetGameObject_ThisIsPtr,
				"get_gameObject", 0,
				"get_gameObject_Injected", 1,
				{
					UNITY_COMPONENT_GETGAMEOBJECT,
					IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_gameObject"),
				},
				{
					IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_gameObject_Injected"),
					IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_gameObject_Injected(System.IntPtr)"),
				});

				resolveInstance(m_ComponentFunctions.m_GetTransform, m_ComponentFunctions.m_GetTransform_ThisIsPtr,
					"get_transform", 0,
					"get_transform_Injected", 1,
					{
						UNITY_COMPONENT_GETTRANSFORM,
						IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_transform"),
					},
				{
					IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_transform_Injected"),
					IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_transform_Injected(System.IntPtr)"),
				});
		}
	}
}
