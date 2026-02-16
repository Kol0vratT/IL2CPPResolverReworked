#pragma once

namespace Unity
{
	struct RigidbodyFunctions_t
	{
		void* m_GetDetectCollisions = nullptr; bool m_GetDetectCollisions_ThisIsPtr = false;
		void* m_SetDetectCollisions = nullptr; bool m_SetDetectCollisions_ThisIsPtr = false;

		void* m_GetVelocity_Injected = nullptr; bool m_GetVelocity_ThisIsPtr = false;
		void* m_GetVelocity_Value = nullptr; bool m_GetVelocityValue_ThisIsPtr = false;

		void* m_SetVelocity_Injected = nullptr; bool m_SetVelocity_ThisIsPtr = false;
		void* m_SetVelocity_Value = nullptr; bool m_SetVelocityValue_ThisIsPtr = false;
	};
	inline RigidbodyFunctions_t m_RigidbodyFunctions;

	class CRigidbody : public IL2CPP::CClass
	{
	public:
		bool GetDetectCollisions()
		{
			if (!this || !m_RigidbodyFunctions.m_GetDetectCollisions)
				return false;

			void* selfArg = m_RigidbodyFunctions.m_GetDetectCollisions_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return false;
			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(void*)>(m_RigidbodyFunctions.m_GetDetectCollisions)(selfArg);
		}

		void SetDetectCollisions(bool m_bDetect)
		{
			if (!this || !m_RigidbodyFunctions.m_SetDetectCollisions)
				return;

			void* selfArg = m_RigidbodyFunctions.m_SetDetectCollisions_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, bool)>(m_RigidbodyFunctions.m_SetDetectCollisions)(selfArg, m_bDetect);
		}

		Vector3 GetVelocity()
		{
			if (!this)
				return {};

			if (m_RigidbodyFunctions.m_GetVelocity_Injected)
			{
				Vector3 v{};
				void* selfArg = m_RigidbodyFunctions.m_GetVelocity_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_RigidbodyFunctions.m_GetVelocity_Injected)(selfArg, v);
				return v;
			}

			if (m_RigidbodyFunctions.m_GetVelocity_Value)
			{
				void* selfArg = m_RigidbodyFunctions.m_GetVelocityValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return {};
				return reinterpret_cast<Vector3(UNITY_CALLING_CONVENTION)(void*)>(m_RigidbodyFunctions.m_GetVelocity_Value)(selfArg);
			}

			return {};
		}

		void SetVelocity(Vector3 m_vVector)
		{
			if (!this)
				return;

			if (m_RigidbodyFunctions.m_SetVelocity_Injected)
			{
				void* selfArg = m_RigidbodyFunctions.m_SetVelocity_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&)>(m_RigidbodyFunctions.m_SetVelocity_Injected)(selfArg, m_vVector);
				return;
			}

			if (m_RigidbodyFunctions.m_SetVelocity_Value)
			{
				void* selfArg = m_RigidbodyFunctions.m_SetVelocityValue_ThisIsPtr ? this->m_CachedPtr : (void*)this;
				if (!selfArg) return;
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3)>(m_RigidbodyFunctions.m_SetVelocity_Value)(selfArg, m_vVector);
			}
		}
	};

	namespace RigidBody
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_RIGIDBODY_CLASS);

			auto resolveInstance = [&](void*& outPtr, bool& outThisIsPtr,
				const char* methodName, int argCount,
				const char* injectedMethodName, int injectedArgCount,
				std::initializer_list<const char*> icallObj,
				std::initializer_list<const char*> icallPtr)
				{
					outPtr = nullptr;
					outThisIsPtr = false;

					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_RIGIDBODY_CLASS, methodName, argCount))
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

					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_RIGIDBODY_CLASS, injectedMethodName, injectedArgCount))
					{
						outPtr = p; outThisIsPtr = true; return;
					}

					if (void* p = IL2CPP::ResolveCallAny(icallPtr))
					{
						outPtr = p; outThisIsPtr = true; return;
					}
				};

			resolveInstance(m_RigidbodyFunctions.m_GetDetectCollisions, m_RigidbodyFunctions.m_GetDetectCollisions_ThisIsPtr,
				"get_detectCollisions", 0,
				"get_detectCollisions_Injected", 1,
				{ UNITY_RIGIDBODY_GETDETECTCOLLISIONS, IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_detectCollisions") },
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_detectCollisions_Injected"),
				  IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_detectCollisions_Injected(System.IntPtr)") });

			resolveInstance(m_RigidbodyFunctions.m_SetDetectCollisions, m_RigidbodyFunctions.m_SetDetectCollisions_ThisIsPtr,
				"set_detectCollisions", 1,
				"set_detectCollisions_Injected", 2,
				{ UNITY_RIGIDBODY_SETDETECTCOLLISIONS, IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_detectCollisions") },
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_detectCollisions_Injected"),
				  IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_detectCollisions_Injected(System.IntPtr,System.Boolean)") });

			// Velocity: prefer injected(out/ref)
			resolveInstance(m_RigidbodyFunctions.m_GetVelocity_Injected, m_RigidbodyFunctions.m_GetVelocity_ThisIsPtr,
				"get_velocity_Injected", 1,
				"get_velocity_Injected", 2,
				{ UNITY_RIGIDBODY_GETVELOCITY, IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_velocity_Injected") },
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_velocity_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_velocity_Injected(System.IntPtr,UnityEngine.Vector3)") });

			resolveInstance(m_RigidbodyFunctions.m_SetVelocity_Injected, m_RigidbodyFunctions.m_SetVelocity_ThisIsPtr,
				"set_velocity_Injected", 1,
				"set_velocity_Injected", 2,
				{ UNITY_RIGIDBODY_SETVELOCITY, IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_velocity_Injected") },
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_velocity_Injected(System.IntPtr,UnityEngine.Vector3&)"),
				  IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_velocity_Injected(System.IntPtr,UnityEngine.Vector3)") });

			// Value-return/by-value fallbacks (optional)
			resolveInstance(m_RigidbodyFunctions.m_GetVelocity_Value, m_RigidbodyFunctions.m_GetVelocityValue_ThisIsPtr,
				"get_velocity", 0,
				"get_velocity_Injected", 1,
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_velocity") },
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_velocity_Injected(System.IntPtr)") });

			resolveInstance(m_RigidbodyFunctions.m_SetVelocity_Value, m_RigidbodyFunctions.m_SetVelocityValue_ThisIsPtr,
				"set_velocity", 1,
				"set_velocity_Injected", 2,
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_velocity") },
				{ IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_velocity_Injected(System.IntPtr)") });
		}
	}
}
