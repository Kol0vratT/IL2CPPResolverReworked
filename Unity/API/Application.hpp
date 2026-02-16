#pragma once

namespace Unity
{
	struct ApplicationFunctions_t
	{
		void* m_GetTargetFrameRate = nullptr;
		void* m_SetTargetFrameRate = nullptr;
		void* m_GetIsFocused = nullptr;

		void* m_GetDataPath = nullptr;
		void* m_GetPersistentDataPath = nullptr;
		void* m_GetStreamingAssetsPath = nullptr;

		void* m_GetProductName = nullptr;
		void* m_GetIdentifier = nullptr;
		void* m_GetUnityVersion = nullptr;

		void* m_Quit = nullptr;
		bool m_QuitTakesExitCode = false;
	};
	inline ApplicationFunctions_t m_ApplicationFunctions;

	namespace Application
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_APPLICATION_CLASS);

			auto resolveStatic = [&](void*& outPtr,
				const char* methodName, int argCount,
				std::initializer_list<const char*> icallNames)
				{
					outPtr = nullptr;

					// 1) managed wrapper
					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_APPLICATION_CLASS, methodName, argCount))
					{
						outPtr = p;
						return;
					}

					// 2) icall fallback
					if (void* p = IL2CPP::ResolveCallAny(icallNames))
					{
						outPtr = p;
						return;
					}
				};

			resolveStatic(m_ApplicationFunctions.m_GetTargetFrameRate,
				"get_targetFrameRate", 0,
				{ UNITY_APPLICATION_GET_TARGETFRAMERATE, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_targetFrameRate") });

			resolveStatic(m_ApplicationFunctions.m_SetTargetFrameRate,
				"set_targetFrameRate", 1,
				{ UNITY_APPLICATION_SET_TARGETFRAMERATE, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::set_targetFrameRate") });

			resolveStatic(m_ApplicationFunctions.m_GetIsFocused,
				"get_isFocused", 0,
				{ UNITY_APPLICATION_GET_ISFOCUSED, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_isFocused") });

			resolveStatic(m_ApplicationFunctions.m_GetDataPath,
				"get_dataPath", 0,
				{ UNITY_APPLICATION_GET_DATAPATH, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_dataPath") });

			resolveStatic(m_ApplicationFunctions.m_GetPersistentDataPath,
				"get_persistentDataPath", 0,
				{ UNITY_APPLICATION_GET_PERSISTENT, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_persistentDataPath") });

			resolveStatic(m_ApplicationFunctions.m_GetStreamingAssetsPath,
				"get_streamingAssetsPath", 0,
				{ UNITY_APPLICATION_GET_STREAMING, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_streamingAssetsPath") });

			resolveStatic(m_ApplicationFunctions.m_GetProductName,
				"get_productName", 0,
				{ UNITY_APPLICATION_GET_PRODUCTNAME, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_productName") });

			resolveStatic(m_ApplicationFunctions.m_GetIdentifier,
				"get_identifier", 0,
				{ UNITY_APPLICATION_GET_IDENTIFIER, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_identifier") });

			resolveStatic(m_ApplicationFunctions.m_GetUnityVersion,
				"get_unityVersion", 0,
				{ UNITY_APPLICATION_GET_UNITYVERSION, IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_unityVersion") });

			m_ApplicationFunctions.m_Quit = nullptr;
			m_ApplicationFunctions.m_QuitTakesExitCode = false;
			if (void* p = IL2CPP::ResolveUnityMethod(UNITY_APPLICATION_CLASS, "Quit", 1))
			{
				m_ApplicationFunctions.m_Quit = p;
				m_ApplicationFunctions.m_QuitTakesExitCode = true;
			}
			else if (void* p = IL2CPP::ResolveCallAny({ UNITY_APPLICATION_QUIT1 }))
			{
				m_ApplicationFunctions.m_Quit = p;
				m_ApplicationFunctions.m_QuitTakesExitCode = true;
			}
			else if (void* p0 = IL2CPP::ResolveUnityMethod(UNITY_APPLICATION_CLASS, "Quit", 0))
			{
				m_ApplicationFunctions.m_Quit = p0;
				m_ApplicationFunctions.m_QuitTakesExitCode = false;
			}
			else if (void* p0 = IL2CPP::ResolveCallAny({ UNITY_APPLICATION_QUIT0 }))
			{
				m_ApplicationFunctions.m_Quit = p0;
				m_ApplicationFunctions.m_QuitTakesExitCode = false;
			}
		}

		// ---------------- Public API ----------------

		inline int GetTargetFrameRate()
		{
			if (!m_ApplicationFunctions.m_GetTargetFrameRate)
				return 0;

			return reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetTargetFrameRate)();
		}

		inline void SetTargetFrameRate(int v)
		{
			if (!m_ApplicationFunctions.m_SetTargetFrameRate)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(int)>(
				m_ApplicationFunctions.m_SetTargetFrameRate)(v);
		}

		inline bool GetIsFocused()
		{
			if (!m_ApplicationFunctions.m_GetIsFocused)
				return false;

			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetIsFocused)();
		}

		inline System_String* GetDataPath()
		{
			if (!m_ApplicationFunctions.m_GetDataPath)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetDataPath)();
		}

		inline System_String* GetPersistentDataPath()
		{
			if (!m_ApplicationFunctions.m_GetPersistentDataPath)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetPersistentDataPath)();
		}

		inline System_String* GetStreamingAssetsPath()
		{
			if (!m_ApplicationFunctions.m_GetStreamingAssetsPath)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetStreamingAssetsPath)();
		}

		inline System_String* GetProductName()
		{
			if (!m_ApplicationFunctions.m_GetProductName)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetProductName)();
		}

		inline System_String* GetIdentifier()
		{
			if (!m_ApplicationFunctions.m_GetIdentifier)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetIdentifier)();
		}

		inline System_String* GetUnityVersion()
		{
			if (!m_ApplicationFunctions.m_GetUnityVersion)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)()>(
				m_ApplicationFunctions.m_GetUnityVersion)();
		}

		inline void Quit(int exitCode = 0)
		{
			if (!m_ApplicationFunctions.m_Quit)
				return;

			if (m_ApplicationFunctions.m_QuitTakesExitCode)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(int)>(m_ApplicationFunctions.m_Quit)(exitCode);
			}
			else
			{
				// fallback Quit()
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)()>(m_ApplicationFunctions.m_Quit)();
			}
		}
	}
}
