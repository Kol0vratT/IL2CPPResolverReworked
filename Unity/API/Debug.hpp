#pragma once

namespace Unity
{
	struct DebugFunctions_t
	{
		void* m_LogObj = nullptr;
		void* m_LogWarningObj = nullptr;
		void* m_LogErrorObj = nullptr;

		// optional string overloads (если вдруг найдутся)
		void* m_LogStr = nullptr;
		void* m_LogWarningStr = nullptr;
		void* m_LogErrorStr = nullptr;
	};
	inline DebugFunctions_t m_DebugFunctions;

	namespace Debug
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_DEBUG_CLASS);

			auto resolveStatic = [&](void*& outPtr,
				const char* methodName, int argCount,
				std::initializer_list<const char*> icallNames)
				{
					outPtr = nullptr;

					// 1) managed wrapper
					if (void* p = IL2CPP::ResolveUnityMethod(UNITY_DEBUG_CLASS, methodName, argCount))
					{
						outPtr = p; return;
					}

					// 2) icall fallback
					if (void* p = IL2CPP::ResolveCallAny(icallNames))
					{
						outPtr = p; return;
					}
				};

			resolveStatic(m_DebugFunctions.m_LogObj,
				"Log", 1,
				{ UNITY_DEBUG_LOG_OBJ, IL2CPP_RStr(UNITY_DEBUG_CLASS"::Log") });

			resolveStatic(m_DebugFunctions.m_LogWarningObj,
				"LogWarning", 1,
				{ UNITY_DEBUG_LOGWARN_OBJ, IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogWarning") });

			resolveStatic(m_DebugFunctions.m_LogErrorObj,
				"LogError", 1,
				{ UNITY_DEBUG_LOGERR_OBJ, IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogError") });

			resolveStatic(m_DebugFunctions.m_LogStr,
				"Log", 1,
				{ UNITY_DEBUG_LOG_STR });

			resolveStatic(m_DebugFunctions.m_LogWarningStr,
				"LogWarning", 1,
				{ UNITY_DEBUG_LOGWARN_STR });

			resolveStatic(m_DebugFunctions.m_LogErrorStr,
				"LogError", 1,
				{ UNITY_DEBUG_LOGERR_STR });
		}

		// ---------------- Public API ----------------

		inline void Log(il2cppObject* msg)
		{
			if (!m_DebugFunctions.m_LogObj) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(il2cppObject*)>(
				m_DebugFunctions.m_LogObj)(msg);
		}

		inline void LogWarning(il2cppObject* msg)
		{
			if (!m_DebugFunctions.m_LogWarningObj) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(il2cppObject*)>(
				m_DebugFunctions.m_LogWarningObj)(msg);
		}

		inline void LogError(il2cppObject* msg)
		{
			if (!m_DebugFunctions.m_LogErrorObj) return;
			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(il2cppObject*)>(
				m_DebugFunctions.m_LogErrorObj)(msg);
		}

		inline void Log(const char* text)
		{
			System_String* s = IL2CPP::String::New(text);

			if (m_DebugFunctions.m_LogStr)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(
					m_DebugFunctions.m_LogStr)(s);
				return;
			}

			Log(reinterpret_cast<il2cppObject*>(s));
		}

		inline void LogWarning(const char* text)
		{
			System_String* s = IL2CPP::String::New(text);

			if (m_DebugFunctions.m_LogWarningStr)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(
					m_DebugFunctions.m_LogWarningStr)(s);
				return;
			}

			LogWarning(reinterpret_cast<il2cppObject*>(s));
		}

		inline void LogError(const char* text)
		{
			System_String* s = IL2CPP::String::New(text);

			if (m_DebugFunctions.m_LogErrorStr)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(
					m_DebugFunctions.m_LogErrorStr)(s);
				return;
			}

			LogError(reinterpret_cast<il2cppObject*>(s));
		}
	}
}
