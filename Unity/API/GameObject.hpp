#pragma once

namespace Unity
{
	struct GameObjectFunctions_t
	{
		void* m_AddComponent = nullptr; bool m_AddComponent_ThisIsPtr = false;
		void* m_CreatePrimitive = nullptr;
		void* m_Find = nullptr;
		void* m_FindGameObjectsWithTag = nullptr;
		void* m_GetComponent = nullptr; bool m_GetComponent_ThisIsPtr = false;
		void* m_GetComponents = nullptr; bool m_GetComponents_ThisIsPtr = false;
		void* m_GetComponentInChildren = nullptr; bool m_GetComponentInChildren_ThisIsPtr = false;
		void* m_GetActive = nullptr; bool m_GetActive_ThisIsPtr = false;
		void* m_GetLayer = nullptr; bool m_GetLayer_ThisIsPtr = false;
		void* m_GetTransform = nullptr; bool m_GetTransform_ThisIsPtr = false;
		void* m_SetActive = nullptr; bool m_SetActive_ThisIsPtr = false;
		void* m_SetLayer = nullptr; bool m_SetLayer_ThisIsPtr = false;
	};
	GameObjectFunctions_t m_GameObjectFunctions;

	class CGameObject : public CObject
	{
	public:
		void AddComponent(il2cppObject* m_pSystemType)
		{
			if (!this || !m_GameObjectFunctions.m_AddComponent || !m_pSystemType)
				return;

			void* selfArg = m_GameObjectFunctions.m_AddComponent_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, void*)>(m_GameObjectFunctions.m_AddComponent)(selfArg, m_pSystemType);
		}

		CComponent* GetComponent(const char* m_pName)
		{
			if (!this || !m_GameObjectFunctions.m_GetComponent || !m_pName)
				return nullptr;

			System_String* name = IL2CPP::String::New(m_pName);
			if (!name)
				return nullptr;

			void* selfArg = m_GameObjectFunctions.m_GetComponent_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CComponent * (UNITY_CALLING_CONVENTION)(void*, System_String*)>(m_GameObjectFunctions.m_GetComponent)(selfArg, name);
		}

		CComponent* GetComponentInChildren(il2cppObject* m_pSystemType, bool includeInactive)
		{
			if (!this || !m_GameObjectFunctions.m_GetComponentInChildren || !m_pSystemType)
				return nullptr;

			void* selfArg = m_GameObjectFunctions.m_GetComponentInChildren_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CComponent * (UNITY_CALLING_CONVENTION)(void*, void*, bool)>(m_GameObjectFunctions.m_GetComponentInChildren)(selfArg, m_pSystemType, includeInactive);
		}

		// e.g CGameObject->GetComponentInChildren("Namespace.Component");
		CComponent* GetComponentInChildren(const char* type)
		{
			if (!type)
				return nullptr;

			il2cppClass* m_pClass = IL2CPP::Class::Find(type);
			if (!m_pClass) return nullptr;

			return GetComponentInChildren(IL2CPP::Class::GetSystemType(m_pClass), true);
		}

		il2cppArray<CComponent*>* GetComponents(il2cppObject* m_pSystemType)
		{
			if (!this || !m_GameObjectFunctions.m_GetComponents || !m_pSystemType)
				return nullptr;

			/*
			0 - Object
			1 - Type
			2 - Use search type as array return type
			3 - Recursive
			4 - Include inactive
			5 - Reverse
			6 - Result list
			*/
			void* selfArg = m_GameObjectFunctions.m_GetComponents_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<Unity::il2cppArray<CComponent*>*(UNITY_CALLING_CONVENTION)(void*, void*, bool, bool, bool, bool, void*)>(m_GameObjectFunctions.m_GetComponents)(selfArg, m_pSystemType, false, false, true, false, nullptr);
		}

		il2cppArray<CComponent*>* GetComponents(const char* m_pSystemTypeName)
		{
			if (!m_pSystemTypeName)
				return nullptr;

			il2cppClass* m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
			if (!m_pClass) return nullptr;

			return GetComponents(IL2CPP::Class::GetSystemType(m_pClass));
		}

		CComponent* GetComponentByIndex(il2cppObject* m_pSystemType, unsigned int m_uIndex = 0U)
		{
			il2cppArray<CComponent*>* m_pComponents = GetComponents(m_pSystemType);
			if (!m_pComponents || m_uIndex >= m_pComponents->m_uMaxLength) return nullptr;

			return m_pComponents->operator[](m_uIndex);
		}

		CComponent* GetComponentByIndex(const char* m_pSystemTypeName, unsigned int m_uIndex = 0U)
		{
			if (!m_pSystemTypeName)
				return nullptr;

			il2cppClass* m_pClass = IL2CPP::Class::Find(m_pSystemTypeName);
			if (!m_pClass) return nullptr;

			return GetComponentByIndex(IL2CPP::Class::GetSystemType(m_pClass), m_uIndex);
		}

		CTransform* GetTransform()
		{
			if (!this || !m_GameObjectFunctions.m_GetTransform)
				return nullptr;

			void* selfArg = m_GameObjectFunctions.m_GetTransform_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*)>(m_GameObjectFunctions.m_GetTransform)(selfArg);
		}

		bool GetActive()
		{
			if (!this || !m_GameObjectFunctions.m_GetActive)
				return false;

			void* selfArg = m_GameObjectFunctions.m_GetActive_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return false;
			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(void*)>(m_GameObjectFunctions.m_GetActive)(selfArg);
		}

		unsigned int GetLayer()
		{
			if (!this || !m_GameObjectFunctions.m_GetLayer)
				return 0U;

			void* selfArg = m_GameObjectFunctions.m_GetLayer_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return 0U;
			return reinterpret_cast<unsigned int(UNITY_CALLING_CONVENTION)(void*)>(m_GameObjectFunctions.m_GetLayer)(selfArg);
		}

		/*
		* (WARNING) when you use GameObject::Find and you set the object to unactive, you won't find it anymore with that slow function.
		*/
		void SetActive(bool m_bActive)
		{
			if (!this || !m_GameObjectFunctions.m_SetActive)
				return;

			void* selfArg = m_GameObjectFunctions.m_SetActive_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, bool)>(m_GameObjectFunctions.m_SetActive)(selfArg, m_bActive);
		}

		void SetLayer(unsigned int m_uLayer)
		{
			if (!this || !m_GameObjectFunctions.m_SetLayer)
				return;

			void* selfArg = m_GameObjectFunctions.m_SetLayer_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, unsigned int)>(m_GameObjectFunctions.m_SetLayer)(selfArg, m_uLayer);
		}
	};

	namespace GameObject
	{
		enum class m_ePrimitiveType : int
		{
			Default = 0,
			Sphere = 0,
			Capsule,
			Cylinder,
			Cube,
			Plane,
			Quad,
		};

		void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_GAMEOBJECT_CLASS);

			// Static methods/properties are stable and can use the managed method pointers safely.
			m_GameObjectFunctions.m_CreatePrimitive = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_GAMEOBJECT_CLASS, "CreatePrimitive", 1,
				{ UNITY_GAMEOBJECT_CREATEPRIMITIVE, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::CreatePrimitive_Injected") });

			m_GameObjectFunctions.m_Find = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_GAMEOBJECT_CLASS, "Find", 1,
				{ UNITY_GAMEOBJECT_FIND, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::Find_Injected") });

			m_GameObjectFunctions.m_FindGameObjectsWithTag = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_GAMEOBJECT_CLASS, "FindGameObjectsWithTag", 1,
				{ UNITY_GAMEOBJECT_FINDGAMEOBJECTWITHTAG, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::FindGameObjectsWithTag_Injected") });

			auto resolveInstance = [&](void*& outPtr, bool& outThisIsPtr,
				const char* methodName, int argCount,
				const char* injectedMethodName, int injectedArgCount,
				std::initializer_list<const char*> icallObj,
				std::initializer_list<const char*> icallPtr)
				{
					outPtr = nullptr;
					outThisIsPtr = false;

					// 1) Prefer managed wrapper (expects UnityEngine.GameObject 'this')
					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_GAMEOBJECT_CLASS, methodName, argCount))
					{
						outPtr = p;
						return;
					}

					// 2) Try old-style icall (still used by some Unity versions)
					for (const char* n : icallObj)
					{
						if (!n)
							continue;
						if (void* p = IL2CPP::ResolveCallCached(n))
						{
							outPtr = p;
							outThisIsPtr = false;
							return;
						}
					}

					// 3) Unity 6+: injected wrappers frequently take a System.IntPtr as the first argument
					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_GAMEOBJECT_CLASS, injectedMethodName, injectedArgCount))
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

			resolveInstance(m_GameObjectFunctions.m_AddComponent, m_GameObjectFunctions.m_AddComponent_ThisIsPtr,
				"Internal_AddComponentWithType", 1,
				"Internal_AddComponentWithType_Injected", 2,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::Internal_AddComponentWithType") },
				{ UNITY_GAMEOBJECT_ADDCOMPONENT, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::Internal_AddComponentWithType_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::Internal_AddComponentWithType_Injected(System.IntPtr,System.Type)") });

			resolveInstance(m_GameObjectFunctions.m_GetComponent, m_GameObjectFunctions.m_GetComponent_ThisIsPtr,
				"GetComponentByName", 1,
				"GetComponentByName_Injected", 2,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentByName") },
				{ UNITY_GAMEOBJECT_GETCOMPONENT, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentByName_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentByName_Injected(System.IntPtr,System.String)") });

			resolveInstance(m_GameObjectFunctions.m_GetComponents, m_GameObjectFunctions.m_GetComponents_ThisIsPtr,
				"GetComponentsInternal", 6,
				"GetComponentsInternal_Injected", 7,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentsInternal") },
				{ UNITY_GAMEOBJECT_GETCOMPONENTS,
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentsInternal_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentsInternal_Injected(System.IntPtr,System.Type,System.Boolean,System.Boolean,System.Boolean,System.Boolean,System.Object)") });

			resolveInstance(m_GameObjectFunctions.m_GetComponentInChildren, m_GameObjectFunctions.m_GetComponentInChildren_ThisIsPtr,
				"GetComponentInChildren", 2,
				"GetComponentInChildren_Injected", 3,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentInChildren") },
				{ UNITY_GAMEOBJECT_GETCOMPONENTINCHILDREN, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentInChildren_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentInChildren_Injected(System.IntPtr,System.Type,System.Boolean)") });

			resolveInstance(m_GameObjectFunctions.m_GetActive, m_GameObjectFunctions.m_GetActive_ThisIsPtr,
				"get_active", 0,
				"get_active_Injected", 1,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_active") },
				{ UNITY_GAMEOBJECT_GETACTIVE, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_active_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_active_Injected(System.IntPtr)") });

			resolveInstance(m_GameObjectFunctions.m_GetLayer, m_GameObjectFunctions.m_GetLayer_ThisIsPtr,
				"get_layer", 0,
				"get_layer_Injected", 1,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_layer") },
				{ UNITY_GAMEOBJECT_GETLAYER, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_layer_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_layer_Injected(System.IntPtr)") });

			resolveInstance(m_GameObjectFunctions.m_GetTransform, m_GameObjectFunctions.m_GetTransform_ThisIsPtr,
				"get_transform", 0,
				"get_transform_Injected", 1,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_transform") },
				{ UNITY_GAMEOBJECT_GETTRANSFORM, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_transform_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_transform_Injected(System.IntPtr)") });

			resolveInstance(m_GameObjectFunctions.m_SetActive, m_GameObjectFunctions.m_SetActive_ThisIsPtr,
				"set_active", 1,
				"set_active_Injected", 2,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_active") },
				{ UNITY_GAMEOBJECT_SETACTIVE, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_active_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_active_Injected(System.IntPtr,System.Boolean)") });

			resolveInstance(m_GameObjectFunctions.m_SetLayer, m_GameObjectFunctions.m_SetLayer_ThisIsPtr,
				"set_layer", 1,
				"set_layer_Injected", 2,
				{ IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_layer") },
				{ UNITY_GAMEOBJECT_SETLAYER, IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_layer_Injected"),
				  IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_layer_Injected(System.IntPtr,System.Int32)") });
		}

		CGameObject* CreatePrimitive(m_ePrimitiveType m_Type)
		{
			if (!m_GameObjectFunctions.m_CreatePrimitive)
				return nullptr;

			return reinterpret_cast<CGameObject * (UNITY_CALLING_CONVENTION)(m_ePrimitiveType)>(m_GameObjectFunctions.m_CreatePrimitive)(m_Type);
		}

		CGameObject* Find(const char* m_Name)
		{
			if (!m_GameObjectFunctions.m_Find || !m_Name)
				return nullptr;

			System_String* name = IL2CPP::String::New(m_Name);
			if (!name)
				return nullptr;

			return reinterpret_cast<CGameObject * (UNITY_CALLING_CONVENTION)(System_String*)>(m_GameObjectFunctions.m_Find)(name);
		}

		il2cppArray<CGameObject*>* FindWithTag(const char* m_Tag)
		{
			if (!m_GameObjectFunctions.m_FindGameObjectsWithTag || !m_Tag)
				return nullptr;

			System_String* tag = IL2CPP::String::New(m_Tag);
			if (!tag)
				return nullptr;

			return reinterpret_cast<il2cppArray<CGameObject*>*(UNITY_CALLING_CONVENTION)(void*)>(m_GameObjectFunctions.m_FindGameObjectsWithTag)(tag);
		}
	}
}
