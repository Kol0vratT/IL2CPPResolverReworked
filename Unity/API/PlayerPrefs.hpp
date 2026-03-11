#pragma once

namespace Unity
{
	struct PlayerPrefsFunctions_t
	{
		void* m_GetInt1 = nullptr;
		void* m_GetInt2 = nullptr;
		void* m_GetFloat1 = nullptr;
		void* m_GetFloat2 = nullptr;
		void* m_GetString1 = nullptr;
		void* m_GetString2 = nullptr;
		void* m_SetInt = nullptr;
		void* m_SetFloat = nullptr;
		void* m_SetString = nullptr;
		void* m_HasKey = nullptr;
		void* m_DeleteKey = nullptr;
		void* m_DeleteAll = nullptr;
		void* m_Save = nullptr;
	};
	inline PlayerPrefsFunctions_t m_PlayerPrefsFunctions;

	namespace PlayerPrefs
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_PLAYERPREFS_CLASS);

			auto resolveFloatMethod = [&](void*& outPtr, const char* methodName,
				std::initializer_list<const char*> primarySignature,
				std::initializer_list<const char*> secondarySignature,
				int argCount,
				std::initializer_list<const char*> icallNames)
				{
					outPtr = IL2CPP::ResolveUnityMethodOrIcall(UNITY_PLAYERPREFS_CLASS, methodName, primarySignature, icallNames);
					if (outPtr || secondarySignature.size() == 0U)
						return;

					outPtr = IL2CPP::ResolveUnityMethodOrIcall(UNITY_PLAYERPREFS_CLASS, methodName, secondarySignature, icallNames);
					if (outPtr)
						return;

					outPtr = IL2CPP::ResolveUnityMethodOrIcall(UNITY_PLAYERPREFS_CLASS, methodName, argCount, icallNames);
				};

			m_PlayerPrefsFunctions.m_GetInt1 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "GetInt",
				{ "System.String" },
				{ UNITY_PP_GETINT1 });

			m_PlayerPrefsFunctions.m_GetInt2 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "GetInt",
				{ "System.String", "System.Int32" },
				{ UNITY_PP_GETINT2 });

			resolveFloatMethod(m_PlayerPrefsFunctions.m_GetFloat1,
				"GetFloat",
				{ "System.String" },
				{},
				1,
				{
					UNITY_PP_GETFLOAT1,
					IL2CPP_RStr(UNITY_PLAYERPREFS_CLASS"::GetFloat(System.String)")
				});

			resolveFloatMethod(m_PlayerPrefsFunctions.m_GetFloat2,
				"GetFloat",
				{ "System.String", "System.Single" },
				{ "System.String", "System.Float" },
				2,
				{
					UNITY_PP_GETFLOAT2,
					IL2CPP_RStr(UNITY_PLAYERPREFS_CLASS"::GetFloat(System.String,System.Single)"),
					IL2CPP_RStr(UNITY_PLAYERPREFS_CLASS"::GetFloat(System.String,System.Float)")
				});

			m_PlayerPrefsFunctions.m_GetString1 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "GetString",
				{ "System.String" },
				{ UNITY_PP_GETSTRING1 });

			m_PlayerPrefsFunctions.m_GetString2 = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "GetString",
				{ "System.String", "System.String" },
				{ UNITY_PP_GETSTRING2 });

			m_PlayerPrefsFunctions.m_SetInt = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "SetInt",
				{ "System.String", "System.Int32" },
				{ UNITY_PP_SETINT });

			resolveFloatMethod(m_PlayerPrefsFunctions.m_SetFloat,
				"SetFloat",
				{ "System.String", "System.Single" },
				{ "System.String", "System.Float" },
				2,
				{
					UNITY_PP_SETFLOAT,
					IL2CPP_RStr(UNITY_PLAYERPREFS_CLASS"::SetFloat(System.String,System.Single)"),
					IL2CPP_RStr(UNITY_PLAYERPREFS_CLASS"::SetFloat(System.String,System.Float)")
				});

			m_PlayerPrefsFunctions.m_SetString = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "SetString",
				{ "System.String", "System.String" },
				{ UNITY_PP_SETSTRING });

			m_PlayerPrefsFunctions.m_HasKey = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "HasKey",
				{ "System.String" },
				{ UNITY_PP_HASKEY });

			m_PlayerPrefsFunctions.m_DeleteKey = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "DeleteKey",
				{ "System.String" },
				{ UNITY_PP_DELETEKEY });

			m_PlayerPrefsFunctions.m_DeleteAll = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "DeleteAll", 0,
				{ UNITY_PP_DELETEALL });

			m_PlayerPrefsFunctions.m_Save = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_PLAYERPREFS_CLASS, "Save", 0,
				{ UNITY_PP_SAVE });
		}

		inline int GetInt(const char* key, int defaultValue = 0)
		{
			if (!key)
				return defaultValue;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return defaultValue;

			if (m_PlayerPrefsFunctions.m_GetInt2)
				return reinterpret_cast<int(UNITY_CALLING_CONVENTION)(System_String*, int)>(m_PlayerPrefsFunctions.m_GetInt2)(sKey, defaultValue);

			if (m_PlayerPrefsFunctions.m_GetInt1)
				return reinterpret_cast<int(UNITY_CALLING_CONVENTION)(System_String*)>(m_PlayerPrefsFunctions.m_GetInt1)(sKey);

			return defaultValue;
		}

		inline float GetFloat(const char* key, float defaultValue = 0.0f)
		{
			if (!key)
				return defaultValue;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return defaultValue;

			if (m_PlayerPrefsFunctions.m_GetFloat2)
				return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(System_String*, float)>(m_PlayerPrefsFunctions.m_GetFloat2)(sKey, defaultValue);

			if (m_PlayerPrefsFunctions.m_GetFloat1)
				return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(System_String*)>(m_PlayerPrefsFunctions.m_GetFloat1)(sKey);

			return defaultValue;
		}

		inline System_String* GetString(const char* key, const char* defaultValue = "")
		{
			if (!key)
				return nullptr;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return nullptr;

			if (m_PlayerPrefsFunctions.m_GetString2)
			{
				System_String* sDefault = IL2CPP::String::New(defaultValue ? defaultValue : "");
				return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)(System_String*, System_String*)>(m_PlayerPrefsFunctions.m_GetString2)(sKey, sDefault);
			}

			if (m_PlayerPrefsFunctions.m_GetString1)
				return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)(System_String*)>(m_PlayerPrefsFunctions.m_GetString1)(sKey);

			return nullptr;
		}

		inline std::string GetStringUtf8(const char* key, const char* defaultValue = "")
		{
			System_String* s = GetString(key, defaultValue);
			return s ? s->ToString() : std::string(defaultValue ? defaultValue : "");
		}

		inline void SetInt(const char* key, int value)
		{
			if (!m_PlayerPrefsFunctions.m_SetInt || !key)
				return;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*, int)>(m_PlayerPrefsFunctions.m_SetInt)(sKey, value);
		}

		inline void SetFloat(const char* key, float value)
		{
			if (!m_PlayerPrefsFunctions.m_SetFloat || !key)
				return;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*, float)>(m_PlayerPrefsFunctions.m_SetFloat)(sKey, value);
		}

		inline void SetString(const char* key, const char* value)
		{
			if (!m_PlayerPrefsFunctions.m_SetString || !key)
				return;

			System_String* sKey = IL2CPP::String::New(key);
			System_String* sValue = IL2CPP::String::New(value ? value : "");
			if (!sKey || !sValue)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*, System_String*)>(m_PlayerPrefsFunctions.m_SetString)(sKey, sValue);
		}

		inline bool HasKey(const char* key)
		{
			if (!m_PlayerPrefsFunctions.m_HasKey || !key)
				return false;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return false;

			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(System_String*)>(m_PlayerPrefsFunctions.m_HasKey)(sKey);
		}

		inline void DeleteKey(const char* key)
		{
			if (!m_PlayerPrefsFunctions.m_DeleteKey || !key)
				return;

			System_String* sKey = IL2CPP::String::New(key);
			if (!sKey)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(System_String*)>(m_PlayerPrefsFunctions.m_DeleteKey)(sKey);
		}

		inline void DeleteAll()
		{
			if (!m_PlayerPrefsFunctions.m_DeleteAll)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)()>(m_PlayerPrefsFunctions.m_DeleteAll)();
		}

		inline void Save()
		{
			if (!m_PlayerPrefsFunctions.m_Save)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)()>(m_PlayerPrefsFunctions.m_Save)();
		}
	}
}
