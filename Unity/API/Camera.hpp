#pragma once

namespace Unity
{
	struct CameraFunctions_t
	{
		void* m_GetCurrent = nullptr;
		void* m_GetMain = nullptr;

		void* m_GetDepth = nullptr;       bool m_GetDepth_ThisIsPtr = false;
		void* m_SetDepth = nullptr;       bool m_SetDepth_ThisIsPtr = false;

		void* m_GetFieldOfView = nullptr; bool m_GetFieldOfView_ThisIsPtr = false;
		void* m_SetFieldOfView = nullptr; bool m_SetFieldOfView_ThisIsPtr = false;

		// Prefer injected(out) form; keep as-is but allow IntPtr this
		void* m_WorldToScreen = nullptr;  bool m_WorldToScreen_ThisIsPtr = false;
	};
	inline CameraFunctions_t m_CameraFunctions;

	class CCamera : public CGameObject
	{
	public:
		float GetDepth()
		{
			if (!this || !m_CameraFunctions.m_GetDepth)
				return 0.f;

			void* selfArg = m_CameraFunctions.m_GetDepth_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return 0.f;
			return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(void*)>(m_CameraFunctions.m_GetDepth)(selfArg);
		}

		void SetDepth(float m_fValue)
		{
			if (!this || !m_CameraFunctions.m_SetDepth)
				return;

			void* selfArg = m_CameraFunctions.m_SetDepth_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, float)>(m_CameraFunctions.m_SetDepth)(selfArg, m_fValue);
		}

		float GetFieldOfView()
		{
			if (!this || !m_CameraFunctions.m_GetFieldOfView)
				return 0.f;

			void* selfArg = m_CameraFunctions.m_GetFieldOfView_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return 0.f;
			return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(void*)>(m_CameraFunctions.m_GetFieldOfView)(selfArg);
		}

		void SetFieldOfView(float m_fValue)
		{
			if (!this || !m_CameraFunctions.m_SetFieldOfView)
				return;

			void* selfArg = m_CameraFunctions.m_SetFieldOfView_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, float)>(m_CameraFunctions.m_SetFieldOfView)(selfArg, m_fValue);
		}

		void WorldToScreen(Vector3& m_vWorld, Vector3& m_vScreen, int m_iEye = 2)
		{
			if (!this || !m_CameraFunctions.m_WorldToScreen)
			{
				m_vScreen = {};
				return;
			}

			void* selfArg = m_CameraFunctions.m_WorldToScreen_ThisIsPtr ? this->m_CachedPtr : (void*)this;
			if (!selfArg)
			{
				m_vScreen = {};
				return;
			}
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, Vector3&, int, Vector3&)>(m_CameraFunctions.m_WorldToScreen)(selfArg, m_vWorld, m_iEye, m_vScreen);
		}
	};

	namespace Camera
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_CAMERA_CLASS);

			// Static getters
			m_CameraFunctions.m_GetCurrent = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_CAMERA_CLASS, "get_current", 0,
				{ UNITY_CAMERA_GETCURRENT, IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_current_Injected") });

			m_CameraFunctions.m_GetMain = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_CAMERA_CLASS, "get_main", 0,
				{ UNITY_CAMERA_GETMAIN, IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_main_Injected") });

			auto resolveInstance = [&](void*& outPtr, bool& outThisIsPtr,
				const char* methodName, int argCount,
				const char* injectedMethodName, int injectedArgCount,
				std::initializer_list<const char*> icallObj,
				std::initializer_list<const char*> icallPtr)
				{
					outPtr = nullptr;
					outThisIsPtr = false;

					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_CAMERA_CLASS, methodName, argCount))
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

					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_CAMERA_CLASS, injectedMethodName, injectedArgCount))
					{
						outPtr = p; outThisIsPtr = true; return;
					}

					if (void* p = IL2CPP::ResolveCallAny(icallPtr))
					{
						outPtr = p; outThisIsPtr = true; return;
					}
				};

			resolveInstance(m_CameraFunctions.m_GetDepth, m_CameraFunctions.m_GetDepth_ThisIsPtr,
				"get_depth", 0,
				"get_depth_Injected", 1,
				{ UNITY_CAMERA_GETDEPTH, IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_depth") },
				{ IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_depth_Injected"),
				  IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_depth_Injected(System.IntPtr)") });

			resolveInstance(m_CameraFunctions.m_SetDepth, m_CameraFunctions.m_SetDepth_ThisIsPtr,
				"set_depth", 1,
				"set_depth_Injected", 2,
				{ UNITY_CAMERA_SETDEPTH, IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_depth") },
				{ IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_depth_Injected"),
				  IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_depth_Injected(System.IntPtr,System.Single)") });

			resolveInstance(m_CameraFunctions.m_GetFieldOfView, m_CameraFunctions.m_GetFieldOfView_ThisIsPtr,
				"get_fieldOfView", 0,
				"get_fieldOfView_Injected", 1,
				{ UNITY_CAMERA_GETFIELDOFVIEW, IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_fieldOfView") },
				{ IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_fieldOfView_Injected"),
				  IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_fieldOfView_Injected(System.IntPtr)") });

			resolveInstance(m_CameraFunctions.m_SetFieldOfView, m_CameraFunctions.m_SetFieldOfView_ThisIsPtr,
				"set_fieldOfView", 1,
				"set_fieldOfView_Injected", 2,
				{ UNITY_CAMERA_SETFIELDOFVIEW, IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_fieldOfView") },
				{ IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_fieldOfView_Injected"),
				  IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_fieldOfView_Injected(System.IntPtr,System.Single)") });

			// WorldToScreenPoint is typically injected(out) variant in IL2CPP
			resolveInstance(m_CameraFunctions.m_WorldToScreen, m_CameraFunctions.m_WorldToScreen_ThisIsPtr,
				"WorldToScreenPoint_Injected", 3,       // (Vector3, int, out Vector3) OR (this, v, eye, out)
				"WorldToScreenPoint_Injected", 4,       // if IntPtr this is explicit in managed injected wrapper
				{ UNITY_CAMERA_WORLDTOSCREEN, IL2CPP_RStr(UNITY_CAMERA_CLASS"::WorldToScreenPoint_Injected") },
				{ IL2CPP_RStr(UNITY_CAMERA_CLASS"::WorldToScreenPoint_Injected"),
				  IL2CPP_RStr(UNITY_CAMERA_CLASS"::WorldToScreenPoint_Injected(System.IntPtr,UnityEngine.Vector3&,System.Int32,UnityEngine.Vector3&)") });
		}

		inline CCamera* GetCurrent()
		{
			if (!m_CameraFunctions.m_GetCurrent)
				return nullptr;

			return reinterpret_cast<CCamera * (UNITY_CALLING_CONVENTION)()>(m_CameraFunctions.m_GetCurrent)();
		}

		inline CCamera* GetMain()
		{
			if (!m_CameraFunctions.m_GetMain)
				return nullptr;

			return reinterpret_cast<CCamera * (UNITY_CALLING_CONVENTION)()>(m_CameraFunctions.m_GetMain)();
		}
	}

	enum m_eCameraType : int
	{
		m_eCameraType_Game = 1,
		m_eCameraType_SceneView = 2,
		m_eCameraType_Preview = 4,
		m_eCameraType_VR = 8,
		m_eCameraType_Reflection = 16,
	};

	enum m_eCameraEye : int
	{
		m_eCameraEye_Left = 0,
		m_eCameraEye_Right = 1,
		m_eCameraEye_Center = 2,
	};
}
