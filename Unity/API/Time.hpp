#pragma once

namespace Unity
{
    struct TimeFunctions_t
    {
        void* m_GetDeltaTime = nullptr;
        void* m_GetUnscaledDeltaTime = nullptr;

        void* m_GetTime = nullptr;
        void* m_GetUnscaledTime = nullptr;

        void* m_GetFixedDeltaTime = nullptr;
        void* m_SetFixedDeltaTime = nullptr;

        void* m_GetTimeScale = nullptr;
        void* m_SetTimeScale = nullptr;

        void* m_GetFrameCount = nullptr;
    };
    inline TimeFunctions_t m_TimeFunctions;

    namespace Time
    {
        inline void Initialize()
        {
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_TIME_CLASS);

            auto resolveStatic = [&](void*& outPtr,
                const char* methodName, int argCount,
                std::initializer_list<const char*> icallNames)
                {
                    outPtr = nullptr;

                    // 1) Prefer managed wrapper (stable)
                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_TIME_CLASS, methodName, argCount))
                    {
                        outPtr = p;
                        return;
                    }

                    // 2) Fall back to icall
                    if (void* p = IL2CPP::ResolveCallAny(icallNames))
                    {
                        outPtr = p;
                        return;
                    }
                };

            resolveStatic(m_TimeFunctions.m_GetDeltaTime,
                "get_deltaTime", 0,
                { UNITY_TIME_GET_DELTATIME, IL2CPP_RStr(UNITY_TIME_CLASS"::get_deltaTime") });

            resolveStatic(m_TimeFunctions.m_GetUnscaledDeltaTime,
                "get_unscaledDeltaTime", 0,
                { UNITY_TIME_GET_UNSCALEDDT, IL2CPP_RStr(UNITY_TIME_CLASS"::get_unscaledDeltaTime") });

            resolveStatic(m_TimeFunctions.m_GetTime,
                "get_time", 0,
                { UNITY_TIME_GET_TIME, IL2CPP_RStr(UNITY_TIME_CLASS"::get_time") });

            resolveStatic(m_TimeFunctions.m_GetUnscaledTime,
                "get_unscaledTime", 0,
                { UNITY_TIME_GET_UNSCALEDTIME, IL2CPP_RStr(UNITY_TIME_CLASS"::get_unscaledTime") });

            resolveStatic(m_TimeFunctions.m_GetFixedDeltaTime,
                "get_fixedDeltaTime", 0,
                { UNITY_TIME_GET_FIXEDDT, IL2CPP_RStr(UNITY_TIME_CLASS"::get_fixedDeltaTime") });

            resolveStatic(m_TimeFunctions.m_SetFixedDeltaTime,
                "set_fixedDeltaTime", 1,
                { UNITY_TIME_SET_FIXEDDT, IL2CPP_RStr(UNITY_TIME_CLASS"::set_fixedDeltaTime") });

            resolveStatic(m_TimeFunctions.m_GetTimeScale,
                "get_timeScale", 0,
                { UNITY_TIME_GET_TIMESCALE, IL2CPP_RStr(UNITY_TIME_CLASS"::get_timeScale") });

            resolveStatic(m_TimeFunctions.m_SetTimeScale,
                "set_timeScale", 1,
                { UNITY_TIME_SET_TIMESCALE, IL2CPP_RStr(UNITY_TIME_CLASS"::set_timeScale") });

            resolveStatic(m_TimeFunctions.m_GetFrameCount,
                "get_frameCount", 0,
                { UNITY_TIME_GET_FRAMECOUNT, IL2CPP_RStr(UNITY_TIME_CLASS"::get_frameCount") });
        }

        // ------------- Public API -------------

        inline float GetDeltaTime()
        {
            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetDeltaTime)();
        }

        inline float GetUnscaledDeltaTime()
        {
            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetUnscaledDeltaTime)();
        }

        inline float GetTime()
        {
            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetTime)();
        }

        inline float GetUnscaledTime()
        {
            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetUnscaledTime)();
        }

        inline float GetFixedDeltaTime()
        {
            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetFixedDeltaTime)();
        }

        inline void SetFixedDeltaTime(float v)
        {
            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(float)>(
                m_TimeFunctions.m_SetFixedDeltaTime)(v);
        }

        inline float GetTimeScale()
        {
            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetTimeScale)();
        }

        inline void SetTimeScale(float v)
        {
            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(float)>(
                m_TimeFunctions.m_SetTimeScale)(v);
        }

        inline int GetFrameCount()
        {
            return reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
                m_TimeFunctions.m_GetFrameCount)();
        }
    }
}
