#pragma once

namespace Unity
{
	struct DebugFunctions_t
	{
		void* m_LogObj = nullptr;
		void* m_LogWarningObj = nullptr;
		void* m_LogErrorObj = nullptr;

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

			m_DebugFunctions.m_LogObj = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_DEBUG_CLASS, "Log",
				{ "System.Object" },
				{ UNITY_DEBUG_LOG_OBJ, IL2CPP_RStr(UNITY_DEBUG_CLASS"::Log") });

			m_DebugFunctions.m_LogWarningObj = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_DEBUG_CLASS, "LogWarning",
				{ "System.Object" },
				{ UNITY_DEBUG_LOGWARN_OBJ, IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogWarning") });

			m_DebugFunctions.m_LogErrorObj = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_DEBUG_CLASS, "LogError",
				{ "System.Object" },
				{ UNITY_DEBUG_LOGERR_OBJ, IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogError") });

			m_DebugFunctions.m_LogStr = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_DEBUG_CLASS, "Log",
				{ "System.String" },
				{ UNITY_DEBUG_LOG_STR });

			m_DebugFunctions.m_LogWarningStr = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_DEBUG_CLASS, "LogWarning",
				{ "System.String" },
				{ UNITY_DEBUG_LOGWARN_STR });

			m_DebugFunctions.m_LogErrorStr = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_DEBUG_CLASS, "LogError",
				{ "System.String" },
				{ UNITY_DEBUG_LOGERR_STR });
		}

		inline void Log(il2cppObject* msg)
		{
			if (!m_DebugFunctions.m_LogObj)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(il2cppObject*)>(m_DebugFunctions.m_LogObj)(msg);
		}

		inline void LogWarning(il2cppObject* msg)
		{
			if (!m_DebugFunctions.m_LogWarningObj)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(il2cppObject*)>(m_DebugFunctions.m_LogWarningObj)(msg);
		}

		inline void LogError(il2cppObject* msg)
		{
			if (!m_DebugFunctions.m_LogErrorObj)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(il2cppObject*)>(m_DebugFunctions.m_LogErrorObj)(msg);
		}

		inline void Log(const char* text)
		{
			System_String* s = IL2CPP::String::New(text ? text : "");
			if (!s)
				return;

			if (m_DebugFunctions.m_LogStr)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(m_DebugFunctions.m_LogStr)(s);
				return;
			}

			Log(reinterpret_cast<il2cppObject*>(s));
		}

		inline void LogWarning(const char* text)
		{
			System_String* s = IL2CPP::String::New(text ? text : "");
			if (!s)
				return;

			if (m_DebugFunctions.m_LogWarningStr)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(m_DebugFunctions.m_LogWarningStr)(s);
				return;
			}

			LogWarning(reinterpret_cast<il2cppObject*>(s));
		}

		inline void LogError(const char* text)
		{
			System_String* s = IL2CPP::String::New(text ? text : "");
			if (!s)
				return;

			if (m_DebugFunctions.m_LogErrorStr)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(m_DebugFunctions.m_LogErrorStr)(s);
				return;
			}

			LogError(reinterpret_cast<il2cppObject*>(s));
		}
	}
}
