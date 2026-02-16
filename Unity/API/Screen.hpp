#pragma once

namespace Unity
{
    struct ScreenFunctions_t
    {
        void* m_GetWidth = nullptr;
        void* m_GetHeight = nullptr;
        void* m_GetDpi = nullptr;
        void* m_GetFullScreen = nullptr;
        void* m_SetFullScreen = nullptr;
    };
    inline ScreenFunctions_t m_ScreenFunctions;

    namespace Screen
    {
        inline void Initialize()
        {
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_SCREEN_CLASS);

            auto resolveStatic = [&](void*& outPtr,
                const char* methodName, int argCount,
                std::initializer_list<const char*> icallNames)
                {
                    outPtr = nullptr;

                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_SCREEN_CLASS, methodName, argCount))
                    {
                        outPtr = p;
                        return;
                    }

                    if (void* p = IL2CPP::ResolveCallAny(icallNames))
                    {
                        outPtr = p;
                        return;
                    }
                };

            resolveStatic(m_ScreenFunctions.m_GetWidth,
                "get_width", 0,
                { UNITY_SCREEN_GETWIDTH, IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_width") });

            resolveStatic(m_ScreenFunctions.m_GetHeight,
                "get_height", 0,
                { UNITY_SCREEN_GETHEIGHT, IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_height") });

            resolveStatic(m_ScreenFunctions.m_GetDpi,
                "get_dpi", 0,
                { UNITY_SCREEN_GETDPI, IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_dpi") });

            resolveStatic(m_ScreenFunctions.m_GetFullScreen,
                "get_fullScreen", 0,
                { UNITY_SCREEN_GETFULLSCREEN, IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_fullScreen") });

            resolveStatic(m_ScreenFunctions.m_SetFullScreen,
                "set_fullScreen", 1,
                { UNITY_SCREEN_SETFULLSCREEN, IL2CPP_RStr(UNITY_SCREEN_CLASS"::set_fullScreen") });
        }

        inline int GetWidth()
        {
            if (!m_ScreenFunctions.m_GetWidth)
                return 0;

            return reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
                m_ScreenFunctions.m_GetWidth)();
        }

        inline int GetHeight()
        {
            if (!m_ScreenFunctions.m_GetHeight)
                return 0;

            return reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
                m_ScreenFunctions.m_GetHeight)();
        }

        inline float GetDpi()
        {
            if (!m_ScreenFunctions.m_GetDpi)
                return 0.f;

            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_ScreenFunctions.m_GetDpi)();
        }

        inline bool GetFullScreen()
        {
            if (!m_ScreenFunctions.m_GetFullScreen)
                return false;

            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)()>(
                m_ScreenFunctions.m_GetFullScreen)();
        }

        inline void SetFullScreen(bool value)
        {
            if (!m_ScreenFunctions.m_SetFullScreen)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(bool)>(
                m_ScreenFunctions.m_SetFullScreen)(value);
        }
    }
}
