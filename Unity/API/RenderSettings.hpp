#pragma once

namespace Unity
{
    struct RenderSettingsFunctions_t
    {
        void* m_GetFog = nullptr;
        void* m_SetFog = nullptr;

        void* m_GetFogColor = nullptr;
        void* m_SetFogColor = nullptr;

        void* m_GetAmbientLight = nullptr;
        void* m_SetAmbientLight = nullptr;

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

            // -------------------------
            // helpers for static props
            // -------------------------
            auto resolveStatic = [&](void*& outPtr,
                const char* methodName, int argCount,
                const char* injectedMethodName, int injectedArgCount,
                std::initializer_list<const char*> icallObj,
                std::initializer_list<const char*> icallInjected)
                {
                    outPtr = nullptr;

                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_RENDERSETTINGS_CLASS, methodName, argCount))
                    {
                        outPtr = p; return;
                    }

                    for (const char* n : icallObj)
                    {
                        if (!n) continue;
                        if (void* p = IL2CPP::ResolveCallCached(n))
                        {
                            outPtr = p; return;
                        }
                    }

                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_RENDERSETTINGS_CLASS, injectedMethodName, injectedArgCount))
                    {
                        outPtr = p; return;
                    }

                    if (void* p = IL2CPP::ResolveCallAny(icallInjected))
                    {
                        outPtr = p; return;
                    }
                };

            resolveStatic(m_RenderSettingsFunctions.m_GetFog,
                "get_fog", 0,
                "get_fog_Injected", 1, 
                { UNITY_RENDERSETTINGS_GET_FOG, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fog") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fog_Injected") });

            resolveStatic(m_RenderSettingsFunctions.m_SetFog,
                "set_fog", 1,
                "set_fog_Injected", 2,
                { UNITY_RENDERSETTINGS_SET_FOG, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fog") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fog_Injected") });


            resolveStatic(m_RenderSettingsFunctions.m_GetFogColor,
                "get_fogColor", 0,
                "get_fogColor_Injected", 1,
                { UNITY_RENDERSETTINGS_GET_FOGCOLOR, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor_Injected"),
                  IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor_Injected(UnityEngine.Color&)") });

            resolveStatic(m_RenderSettingsFunctions.m_SetFogColor,
                "set_fogColor", 1,
                "set_fogColor_Injected", 1, 
                { UNITY_RENDERSETTINGS_SET_FOGCOLOR, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor_Injected"),
                  IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor_Injected(UnityEngine.Color&)") });

            resolveStatic(m_RenderSettingsFunctions.m_GetAmbientLight,
                "get_ambientLight", 0,
                "get_ambientLight_Injected", 1,
                { UNITY_RENDERSETTINGS_GET_AMBIENT, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight_Injected"),
                  IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight_Injected(UnityEngine.Color&)") });

            resolveStatic(m_RenderSettingsFunctions.m_SetAmbientLight,
                "set_ambientLight", 1,
                "set_ambientLight_Injected", 1,
                { UNITY_RENDERSETTINGS_SET_AMBIENT, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight_Injected"),
                  IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight_Injected(UnityEngine.Color&)") });


            resolveStatic(m_RenderSettingsFunctions.m_GetSkybox,
                "get_skybox", 0,
                "get_skybox_Injected", 1,
                { UNITY_RENDERSETTINGS_GET_SKYBOX, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_skybox") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_skybox_Injected") });

            resolveStatic(m_RenderSettingsFunctions.m_SetSkybox,
                "set_skybox", 1,
                "set_skybox_Injected", 2,
                { UNITY_RENDERSETTINGS_SET_SKYBOX, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_skybox") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_skybox_Injected") });

            resolveStatic(m_RenderSettingsFunctions.m_GetSun,
                "get_sun", 0,
                "get_sun_Injected", 1,
                { UNITY_RENDERSETTINGS_GET_SUN, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_sun") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_sun_Injected") });

            resolveStatic(m_RenderSettingsFunctions.m_SetSun,
                "set_sun", 1,
                "set_sun_Injected", 2,
                { UNITY_RENDERSETTINGS_SET_SUN, IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_sun") },
                { IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_sun_Injected") });
        }

        // -------------------------
        // Public API (usage)
        // -------------------------
        inline bool GetFog()
        {
            if (!m_RenderSettingsFunctions.m_GetFog)
                return false;

            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)()>(
                m_RenderSettingsFunctions.m_GetFog)();
        }

        inline void SetFog(bool v)
        {
            if (!m_RenderSettingsFunctions.m_SetFog)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(bool)>(
                m_RenderSettingsFunctions.m_SetFog)(v);
        }
        inline Color GetFogColor()
        {
            Color out{};
            if (!m_RenderSettingsFunctions.m_GetFogColor)
                return out;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(
                m_RenderSettingsFunctions.m_GetFogColor)(out);
            return out;
        }

        inline void SetFogColor(Color& v)
        {
            if (!m_RenderSettingsFunctions.m_SetFogColor)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(
                m_RenderSettingsFunctions.m_SetFogColor)(v);
        }

        inline Color GetAmbientLight()
        {
            Color out{};
            if (!m_RenderSettingsFunctions.m_GetAmbientLight)
                return out;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(
                m_RenderSettingsFunctions.m_GetAmbientLight)(out);
            return out;
        }

        inline void SetAmbientLight(Color& v)
        {
            if (!m_RenderSettingsFunctions.m_SetAmbientLight)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Color&)>(
                m_RenderSettingsFunctions.m_SetAmbientLight)(v);
        }

        inline CObject* GetSkybox()
        {
            if (!m_RenderSettingsFunctions.m_GetSkybox)
                return nullptr;

            return reinterpret_cast<CObject * (UNITY_CALLING_CONVENTION)()>(
                m_RenderSettingsFunctions.m_GetSkybox)();
        }

        inline void SetSkybox(CObject* mat)
        {
            if (!m_RenderSettingsFunctions.m_SetSkybox)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(CObject*)>(
                m_RenderSettingsFunctions.m_SetSkybox)(mat);
        }

        inline CObject* GetSun()
        {
            if (!m_RenderSettingsFunctions.m_GetSun)
                return nullptr;

            return reinterpret_cast<CObject * (UNITY_CALLING_CONVENTION)()>(
                m_RenderSettingsFunctions.m_GetSun)();
        }

        inline void SetSun(CObject* lightObj)
        {
            if (!m_RenderSettingsFunctions.m_SetSun)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(CObject*)>(
                m_RenderSettingsFunctions.m_SetSun)(lightObj);
        }
    }
}
