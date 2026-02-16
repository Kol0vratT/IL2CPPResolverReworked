#pragma once

namespace Unity
{
	struct TransformFunctions_t
	{
		void* m_GetParent = nullptr; bool m_GetParent_ThisIsPtr = false;
		void* m_GetRoot = nullptr; bool m_GetRoot_ThisIsPtr = false;
		void* m_GetChild = nullptr; bool m_GetChild_ThisIsPtr = false;
		void* m_GetChildCount = nullptr; bool m_GetChildCount_ThisIsPtr = false;
		void* m_FindChild = nullptr; bool m_FindChild_ThisIsPtr = false;

		// Vector/Quat getters: we support injected(out) and value-return fallbacks
		void* m_GetPosition_Injected = nullptr; bool m_GetPosition_ThisIsPtr = false;
		void* m_GetPosition_Value = nullptr; bool m_GetPositionValue_ThisIsPtr = false;

		void* m_GetRotation_Injected = nullptr; bool m_GetRotation_ThisIsPtr = false;
		void* m_GetRotation_Value = nullptr; bool m_GetRotationValue_ThisIsPtr = false;

		void* m_GetLocalPosition_Injected = nullptr; bool m_GetLocalPosition_ThisIsPtr = false;
		void* m_GetLocalPosition_Value = nullptr; bool m_GetLocalPositionValue_ThisIsPtr = false;

		void* m_GetLocalScale_Injected = nullptr; bool m_GetLocalScale_ThisIsPtr = false;
		void* m_GetLocalScale_Value = nullptr; bool m_GetLocalScaleValue_ThisIsPtr = false;

		// Setters: support injected(ref) and by-value
		void* m_SetPosition_Injected = nullptr; bool m_SetPosition_ThisIsPtr = false;
		void* m_SetPosition_Value = nullptr; bool m_SetPositionValue_ThisIsPtr = false;

		void* m_SetRotation_Injected = nullptr; bool m_SetRotation_ThisIsPtr = false;
		void* m_SetRotation_Value = nullptr; bool m_SetRotationValue_ThisIsPtr = false;

		void* m_SetLocalPosition_Injected = nullptr; bool m_SetLocalPosition_ThisIsPtr = false;
		void* m_SetLocalPosition_Value = nullptr; bool m_SetLocalPositionValue_ThisIsPtr = false;

		void* m_SetLocalScale_Injected = nullptr; bool m_SetLocalScale_ThisIsPtr = false;
		void* m_SetLocalScale_Value = nullptr; bool m_SetLocalScaleValue_ThisIsPtr = false;
	};
	inline TransformFunctions_t m_TransformFunctions;

	class CTransform : public IL2CPP::CClass
	{
	public:
		CTransform* GetParent()
		{
			if (!this || !m_TransformFunctions.m_GetParent)
				return nullptr;

			void* selfArg = m_TransformFunctions.m_GetParent_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetParent)(selfArg);
		}

		CTransform* GetRoot()
		{
			if (!this || !m_TransformFunctions.m_GetRoot)
				return nullptr;

			void* selfArg = m_TransformFunctions.m_GetRoot_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetRoot)(selfArg);
		}

		CTransform* GetChild(int m_iIndex)
		{
			if (!this || !m_TransformFunctions.m_GetChild)
				return nullptr;

			void* selfArg = m_TransformFunctions.m_GetChild_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*, int)>(m_TransformFunctions.m_GetChild)(selfArg, m_iIndex);
		}

		int GetChildCount()
		{
			if (!this || !m_TransformFunctions.m_GetChildCount)
				return 0;

			void* selfArg = m_TransformFunctions.m_GetChildCount_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return 0;
			return reinterpret_cast<int(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetChildCount)(selfArg);
		}

		CTransform* FindChild(const char* path, bool isActiveOnly)
		{
			if (!this || !m_TransformFunctions.m_FindChild || !path)
				return nullptr;

			System_String* s = IL2CPP::String::New(path);
			if (!s)
				return nullptr;

			void* selfArg = m_TransformFunctions.m_FindChild_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return nullptr;
			return reinterpret_cast<CTransform * (UNITY_CALLING_CONVENTION)(void*, System_String*, bool)>(m_TransformFunctions.m_FindChild)(selfArg, s, isActiveOnly);
		}

		CTransform* FindChild(const char* path)
		{
			if (!path) return nullptr;
			return FindChild(path, false);
		}

		Vector3 GetPosition()
		{
			if (!this)
				return {};

			// Prefer injected(out)
			if (m_TransformFunctions.m_GetPosition_Injected)
			{
				Vector3 v{};
				void* selfArg = m_TransformFunctions.m_GetPosition_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_GetPosition_Injected)(selfArg, v);
				return v;
			}

			// Fallback value-return (if present)
			if (m_TransformFunctions.m_GetPosition_Value)
			{
				void* selfArg = m_TransformFunctions.m_GetPositionValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				return reinterpret_cast<Vector3(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetPosition_Value)(selfArg);
			}

			return {};
		}

		Quaternion GetRotation()
		{
			if (!this)
				return {};

			if (m_TransformFunctions.m_GetRotation_Injected)
			{
				Quaternion q{};
				void* selfArg = m_TransformFunctions.m_GetRotation_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Quaternion&)>(m_TransformFunctions.m_GetRotation_Injected)(selfArg, q);
				return q;
			}

			if (m_TransformFunctions.m_GetRotation_Value)
			{
				void* selfArg = m_TransformFunctions.m_GetRotationValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				return reinterpret_cast<Quaternion(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetRotation_Value)(selfArg);
			}

			return {};
		}

		Vector3 GetLocalPosition()
		{
			if (!this)
				return {};

			if (m_TransformFunctions.m_GetLocalPosition_Injected)
			{
				Vector3 v{};
				void* selfArg = m_TransformFunctions.m_GetLocalPosition_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_GetLocalPosition_Injected)(selfArg, v);
				return v;
			}

			if (m_TransformFunctions.m_GetLocalPosition_Value)
			{
				void* selfArg = m_TransformFunctions.m_GetLocalPositionValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				return reinterpret_cast<Vector3(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetLocalPosition_Value)(selfArg);
			}

			return {};
		}

		Vector3 GetLocalScale()
		{
			if (!this)
				return {};

			if (m_TransformFunctions.m_GetLocalScale_Injected)
			{
				Vector3 v{};
				void* selfArg = m_TransformFunctions.m_GetLocalScale_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_GetLocalScale_Injected)(selfArg, v);
				return v;
			}

			if (m_TransformFunctions.m_GetLocalScale_Value)
			{
				void* selfArg = m_TransformFunctions.m_GetLocalScaleValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				return reinterpret_cast<Vector3(UNITY_CALLING_CONVENTION)(void*)>(m_TransformFunctions.m_GetLocalScale_Value)(selfArg);
			}

			return {};
		}

		void SetPosition(Vector3 m_vVector)
		{
			if (!this)
				return;

			// Prefer injected(ref)
			if (m_TransformFunctions.m_SetPosition_Injected)
			{
				void* selfArg = m_TransformFunctions.m_SetPosition_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_SetPosition_Injected)(selfArg, m_vVector);
				return;
			}

			if (m_TransformFunctions.m_SetPosition_Value)
			{
				void* selfArg = m_TransformFunctions.m_SetPositionValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_TransformFunctions.m_SetPosition_Value)(selfArg, m_vVector);
			}
		}

		void SetRotation(Quaternion m_qQuat)
		{
			if (!this)
				return;

			if (m_TransformFunctions.m_SetRotation_Injected)
			{
				void* selfArg = m_TransformFunctions.m_SetRotation_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Quaternion&)>(m_TransformFunctions.m_SetRotation_Injected)(selfArg, m_qQuat);
				return;
			}

			if (m_TransformFunctions.m_SetRotation_Value)
			{
				void* selfArg = m_TransformFunctions.m_SetRotationValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Quaternion)>(m_TransformFunctions.m_SetRotation_Value)(selfArg, m_qQuat);
			}
		}

		void SetLocalPosition(Vector3 m_vVector)
		{
			if (!this)
				return;

			if (m_TransformFunctions.m_SetLocalPosition_Injected)
			{
				void* selfArg = m_TransformFunctions.m_SetLocalPosition_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_SetLocalPosition_Injected)(selfArg, m_vVector);
				return;
			}

			if (m_TransformFunctions.m_SetLocalPosition_Value)
			{
				void* selfArg = m_TransformFunctions.m_SetLocalPositionValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_TransformFunctions.m_SetLocalPosition_Value)(selfArg, m_vVector);
			}
		}

		void SetLocalScale(Vector3 m_vVector)
		{
			if (!this)
				return;

			if (m_TransformFunctions.m_SetLocalScale_Injected)
			{
				void* selfArg = m_TransformFunctions.m_SetLocalScale_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_TransformFunctions.m_SetLocalScale_Injected)(selfArg, m_vVector);
				return;
			}

			if (m_TransformFunctions.m_SetLocalScale_Value)
			{
				void* selfArg = m_TransformFunctions.m_SetLocalScaleValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_TransformFunctions.m_SetLocalScale_Value)(selfArg, m_vVector);
			}
		}
	};

	namespace Transform
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_TRANSFORM_CLASS);

			auto resolveInstanceObjOrPtr = [&](void*& outPtr, bool& outThisIsPtr,
				const char* methodName, int argCount,
				const char* injectedMethodName, int injectedArgCount,
				std::initializer_list<const char*> icallObj,
				std::initializer_list<const char*> icallPtr)
				{
					outPtr = nullptr;
					outThisIsPtr = false;

					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_TRANSFORM_CLASS, methodName, argCount))
					{
						outPtr = p; return;
					}

					for (const char* n : icallObj)
					{
						if (!n) continue;
						if (void* p = IL2CPP::ResolveCallCached(n))
						{
							outPtr = p; outThisIsPtr = false; return;
						}
					}

					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_TRANSFORM_CLASS, injectedMethodName, injectedArgCount))
					{
						outPtr = p; outThisIsPtr = true; return;
					}

					if (void* p = IL2CPP::ResolveCallAny(icallPtr))
					{
						outPtr = p; outThisIsPtr = true; return;
					}
				};

			// Basic instance refs/ints/bools
			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetParent, m_TransformFunctions.m_GetParent_ThisIsPtr,
				"get_parent", 0,
				"get_parent_Injected", 1,
				{ UNITY_TRANSFORM_GETPARENT, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_parent") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_parent_Injected"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_parent_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetRoot, m_TransformFunctions.m_GetRoot_ThisIsPtr,
				"get_root", 0,
				"get_root_Injected", 1,
				{ UNITY_TRANSFORM_GETROOT, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_root") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_root_Injected"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_root_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetChild, m_TransformFunctions.m_GetChild_ThisIsPtr,
				"GetChild", 1,
				"GetChild_Injected", 2,
				{ UNITY_TRANSFORM_GETCHILD, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::GetChild") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::GetChild_Injected"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::GetChild_Injected(System.IntPtr,System.Int32)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetChildCount, m_TransformFunctions.m_GetChildCount_ThisIsPtr,
				"get_childCount", 0,
				"get_childCount_Injected", 1,
				{ UNITY_TRANSFORM_GETCHILDCOUNT, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_childCount") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_childCount_Injected"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_childCount_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_FindChild, m_TransformFunctions.m_FindChild_ThisIsPtr,
				"Find", 2,
				"Find_Injected", 3,
				{ UNITY_TRANSFORM_FINDCHILD, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::Find") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::Find_Injected"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::Find_Injected(System.IntPtr,System.String,System.Boolean)") });

			// Vector/Quat getters: prefer injected(out), fallback to value-return
			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetPosition_Injected, m_TransformFunctions.m_GetPosition_ThisIsPtr,
				"get_position_Injected", 1,
				"get_position_Injected", 2, // if injected expects IntPtr, it becomes (IntPtr, out Vector3)
				{ UNITY_TRANSFORM_GETPOSITION, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_position_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_position_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_position_Injected(System.IntPtr,UnityEngine.Vector3)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetRotation_Injected, m_TransformFunctions.m_GetRotation_ThisIsPtr,
				"get_rotation_Injected", 1,
				"get_rotation_Injected", 2,
				{ UNITY_TRANSFORM_GETROTATION, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_rotation_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_rotation_Injected(System.IntPtr,UnityEngine.Quaternion&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_rotation_Injected(System.IntPtr,UnityEngine.Quaternion)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetLocalPosition_Injected, m_TransformFunctions.m_GetLocalPosition_ThisIsPtr,
				"get_localPosition_Injected", 1,
				"get_localPosition_Injected", 2,
				{ UNITY_TRANSFORM_GETLOCALPOSITION, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localPosition_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localPosition_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localPosition_Injected(System.IntPtr,UnityEngine.Vector3)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetLocalScale_Injected, m_TransformFunctions.m_GetLocalScale_ThisIsPtr,
				"get_localScale_Injected", 1,
				"get_localScale_Injected", 2,
				{ UNITY_TRANSFORM_GETLOCALSCALE, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localScale_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localScale_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localScale_Injected(System.IntPtr,UnityEngine.Vector3)") });

			// Value-return fallbacks (rarely needed; included for completeness)
			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetPosition_Value, m_TransformFunctions.m_GetPositionValue_ThisIsPtr,
				"get_position", 0,
				"get_position_Injected", 1,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_position") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_position_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetRotation_Value, m_TransformFunctions.m_GetRotationValue_ThisIsPtr,
				"get_rotation", 0,
				"get_rotation_Injected", 1,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_rotation") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_rotation_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetLocalPosition_Value, m_TransformFunctions.m_GetLocalPositionValue_ThisIsPtr,
				"get_localPosition", 0,
				"get_localPosition_Injected", 1,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localPosition") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localPosition_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_GetLocalScale_Value, m_TransformFunctions.m_GetLocalScaleValue_ThisIsPtr,
				"get_localScale", 0,
				"get_localScale_Injected", 1,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localScale") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localScale_Injected(System.IntPtr)") });

			// Setters: prefer injected(ref), fallback to by-value
			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetPosition_Injected, m_TransformFunctions.m_SetPosition_ThisIsPtr,
				"set_position_Injected", 1,
				"set_position_Injected", 2,
				{ UNITY_TRANSFORM_SETPOSITION, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_position_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_position_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_position_Injected(System.IntPtr,UnityEngine.Vector3)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetRotation_Injected, m_TransformFunctions.m_SetRotation_ThisIsPtr,
				"set_rotation_Injected", 1,
				"set_rotation_Injected", 2,
				{ UNITY_TRANSFORM_SETROTATION, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_rotation_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_rotation_Injected(System.IntPtr,UnityEngine.Quaternion&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_rotation_Injected(System.IntPtr,UnityEngine.Quaternion)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetLocalPosition_Injected, m_TransformFunctions.m_SetLocalPosition_ThisIsPtr,
				"set_localPosition_Injected", 1,
				"set_localPosition_Injected", 2,
				{ UNITY_TRANSFORM_SETLOCALPOSITION, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localPosition_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localPosition_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localPosition_Injected(System.IntPtr,UnityEngine.Vector3)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetLocalScale_Injected, m_TransformFunctions.m_SetLocalScale_ThisIsPtr,
				"set_localScale_Injected", 1,
				"set_localScale_Injected", 2,
				{ UNITY_TRANSFORM_SETLOCALSCALE, IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localScale_Injected") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localScale_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localScale_Injected(System.IntPtr,UnityEngine.Vector3)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetPosition_Value, m_TransformFunctions.m_SetPositionValue_ThisIsPtr,
				"set_position", 1,
				"set_position_Injected", 2,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_position") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_position_Injected(System.IntPtr,System.Single,System.Single,System.Single)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetRotation_Value, m_TransformFunctions.m_SetRotationValue_ThisIsPtr,
				"set_rotation", 1,
				"set_rotation_Injected", 2,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_rotation") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_rotation_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetLocalPosition_Value, m_TransformFunctions.m_SetLocalPositionValue_ThisIsPtr,
				"set_localPosition", 1,
				"set_localPosition_Injected", 2,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localPosition") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localPosition_Injected(System.IntPtr)") });

			resolveInstanceObjOrPtr(m_TransformFunctions.m_SetLocalScale_Value, m_TransformFunctions.m_SetLocalScaleValue_ThisIsPtr,
				"set_localScale", 1,
				"set_localScale_Injected", 2,
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localScale") },
				{ IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localScale_Injected(System.IntPtr)") });
		}
	}
}
