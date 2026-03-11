#pragma once

namespace Unity
{
	struct RenderSettingsFunctions_t
	{
		void* m_GetFog = nullptr;
		void* m_SetFog = nullptr;

		void* m_GetFogColor_Value = nullptr;
		void* m_GetFogColor_Injected = nullptr;
		void* m_SetFogColor_Value = nullptr;
		void* m_SetFogColor_Injected = nullptr;

		void* m_GetAmbientLight_Value = nullptr;
		void* m_GetAmbientLight_Injected = nullptr;
		void* m_SetAmbientLight_Value = nullptr;
		void* m_SetAmbientLight_Injected = nullptr;

		void* m_GetSkybox = nullptr;
		void* m_SetSkybox = nullptr;

		void* m_GetSun = nullptr;
		void* m_SetSun = nullptr;
	};
	inline RenderSettingsFunctions_t m_RenderSettingsFunctions;

	namespace RenderSettings
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_RENDERSETTINGS_CLASS);

			m_RenderSettingsFunctions.m_GetFog = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_fog", 0,
				{ UNITY_RENDERSETTINGS_GET_FOG, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fog") });

			m_RenderSettingsFunctions.m_SetFog = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_fog",
				{ "System.Boolean" },
				{ UNITY_RENDERSETTINGS_SET_FOG, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fog") });

			m_RenderSettingsFunctions.m_GetFogColor_Value = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_fogColor", 0,
				{ UNITY_RENDERSETTINGS_GET_FOGCOLOR, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor") });

			m_RenderSettingsFunctions.m_GetFogColor_Injected = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_fogColor_Injected",
				{ "UnityEngine.Color" },
				{
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor_Injected"),
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor_Injected(UnityEngine.Color&)"),
				});

			m_RenderSettingsFunctions.m_SetFogColor_Value = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_fogColor",
				{ "UnityEngine.Color" },
				{ UNITY_RENDERSETTINGS_SET_FOGCOLOR, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor") });

			m_RenderSettingsFunctions.m_SetFogColor_Injected = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_fogColor_Injected",
				{ "UnityEngine.Color" },
				{
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor_Injected"),
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor_Injected(UnityEngine.Color&)"),
				});

			m_RenderSettingsFunctions.m_GetAmbientLight_Value = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_ambientLight", 0,
				{ UNITY_RENDERSETTINGS_GET_AMBIENT, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight") });

			m_RenderSettingsFunctions.m_GetAmbientLight_Injected = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_ambientLight_Injected",
				{ "UnityEngine.Color" },
				{
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight_Injected"),
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight_Injected(UnityEngine.Color&)"),
				});

			m_RenderSettingsFunctions.m_SetAmbientLight_Value = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_ambientLight",
				{ "UnityEngine.Color" },
				{ UNITY_RENDERSETTINGS_SET_AMBIENT, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight") });

			m_RenderSettingsFunctions.m_SetAmbientLight_Injected = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_ambientLight_Injected",
				{ "UnityEngine.Color" },
				{
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight_Injected"),
					IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight_Injected(UnityEngine.Color&)"),
				});

			m_RenderSettingsFunctions.m_GetSkybox = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_skybox", 0,
				{ UNITY_RENDERSETTINGS_GET_SKYBOX, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_skybox") });

			m_RenderSettingsFunctions.m_SetSkybox = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_skybox", 1,
				{ UNITY_RENDERSETTINGS_SET_SKYBOX, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_skybox") });

			m_RenderSettingsFunctions.m_GetSun = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "get_sun", 0,
				{ UNITY_RENDERSETTINGS_GET_SUN, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_sun") });

			m_RenderSettingsFunctions.m_SetSun = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_RENDERSETTINGS_CLASS, "set_sun", 1,
				{ UNITY_RENDERSETTINGS_SET_SUN, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_sun") });
		}

		inline bool GetFog()
		{
			if (!m_RenderSettingsFunctions.m_GetFog)
				return false;

			return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)()>(m_RenderSettingsFunctions.m_GetFog)();
		}

		inline void SetFog(bool v)
		{
			if (!m_RenderSettingsFunctions.m_SetFog)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(bool)>(m_RenderSettingsFunctions.m_SetFog)(v);
		}

		inline Color GetFogColor()
		{
			if (m_RenderSettingsFunctions.m_GetFogColor_Injected)
			{
				Color out{};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(m_RenderSettingsFunctions.m_GetFogColor_Injected)(out);
				return out;
			}

			if (m_RenderSettingsFunctions.m_GetFogColor_Value)
				return reinterpret_cast<Color(UNITY_CALLING_CONVENTION)()>(m_RenderSettingsFunctions.m_GetFogColor_Value)();

			return {};
		}

		inline void SetFogColor(Color v)
		{
			if (m_RenderSettingsFunctions.m_SetFogColor_Injected)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(m_RenderSettingsFunctions.m_SetFogColor_Injected)(v);
				return;
			}

			if (m_RenderSettingsFunctions.m_SetFogColor_Value)
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color)>(m_RenderSettingsFunctions.m_SetFogColor_Value)(v);
		}

		inline Color GetAmbientLight()
		{
			if (m_RenderSettingsFunctions.m_GetAmbientLight_Injected)
			{
				Color out{};
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(m_RenderSettingsFunctions.m_GetAmbientLight_Injected)(out);
				return out;
			}

			if (m_RenderSettingsFunctions.m_GetAmbientLight_Value)
				return reinterpret_cast<Color(UNITY_CALLING_CONVENTION)()>(m_RenderSettingsFunctions.m_GetAmbientLight_Value)();

			return {};
		}

		inline void SetAmbientLight(Color v)
		{
			if (m_RenderSettingsFunctions.m_SetAmbientLight_Injected)
			{
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(m_RenderSettingsFunctions.m_SetAmbientLight_Injected)(v);
				return;
			}

			if (m_RenderSettingsFunctions.m_SetAmbientLight_Value)
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color)>(m_RenderSettingsFunctions.m_SetAmbientLight_Value)(v);
		}

		inline CObject* GetSkybox()
		{
			if (!m_RenderSettingsFunctions.m_GetSkybox)
				return nullptr;

			return reinterpret_cast<CObject * (UNITY_CALLING_CONVENTION)()>(m_RenderSettingsFunctions.m_GetSkybox)();
		}

		inline void SetSkybox(CObject* mat)
		{
			if (!m_RenderSettingsFunctions.m_SetSkybox)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(CObject*)>(m_RenderSettingsFunctions.m_SetSkybox)(mat);
		}

		inline CObject* GetSun()
		{
			if (!m_RenderSettingsFunctions.m_GetSun)
				return nullptr;

			return reinterpret_cast<CObject * (UNITY_CALLING_CONVENTION)()>(m_RenderSettingsFunctions.m_GetSun)();
		}

		inline void SetSun(CObject* lightObj)
		{
			if (!m_RenderSettingsFunctions.m_SetSun)
				return;

			reinterpret_cast<void(UNITY_CALLING_CONVENTION)(CObject*)>(m_RenderSettingsFunctions.m_SetSun)(lightObj);
		}
	}
}
