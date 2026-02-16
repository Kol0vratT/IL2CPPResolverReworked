#pragma once

namespace Unity
{
    struct CursorFunctions_t
    {
        void* m_GetVisible = nullptr;
        void* m_SetVisible = nullptr;

        void* m_GetLockState = nullptr;
        void* m_SetLockState = nullptr;
    };
    inline CursorFunctions_t m_CursorFunctions;

    namespace Cursor
    {
        enum class m_eLockMode : int
        {
            None = 0,
            Locked = 1,
            Confined = 2,
        };

        inline void Initialize()
        {
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_CURSOR_CLASS);

            auto resolveStatic = [&](void*& outPtr,
                const char* methodName, int argCount,
                std::initializer_list<const char*> icallNames)
                {
                    outPtr = nullptr;

                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_CURSOR_CLASS, methodName, argCount))
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

            resolveStatic(m_CursorFunctions.m_GetVisible,
                "get_visible", 0,
                { UNITY_CURSOR_GETVISIBLE, IL2CPP_RStr(UNITY_CURSOR_CLASS"::get_visible") });

            resolveStatic(m_CursorFunctions.m_SetVisible,
                "set_visible", 1,
                { UNITY_CURSOR_SETVISIBLE, IL2CPP_RStr(UNITY_CURSOR_CLASS"::set_visible") });

            resolveStatic(m_CursorFunctions.m_GetLockState,
                "get_lockState", 0,
                { UNITY_CURSOR_GETLOCKSTATE, IL2CPP_RStr(UNITY_CURSOR_CLASS"::get_lockState") });

            resolveStatic(m_CursorFunctions.m_SetLockState,
                "set_lockState", 1,
                { UNITY_CURSOR_SETLOCKSTATE, IL2CPP_RStr(UNITY_CURSOR_CLASS"::set_lockState") });
        }

        inline bool GetVisible()
        {
            if (!m_CursorFunctions.m_GetVisible)
                return false;

            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)()>(
                m_CursorFunctions.m_GetVisible)();
        }

        inline void SetVisible(bool value)
        {
            if (!m_CursorFunctions.m_SetVisible)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(bool)>(
                m_CursorFunctions.m_SetVisible)(value);
        }

        inline m_eLockMode GetLockState()
        {
            if (!m_CursorFunctions.m_GetLockState)
                return m_eLockMode::None;

            return static_cast<m_eLockMode>(
                reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
                    m_CursorFunctions.m_GetLockState)());
        }

        inline void SetLockState(m_eLockMode value)
        {
            if (!m_CursorFunctions.m_SetLockState)
                return;

            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(int)>(
                m_CursorFunctions.m_SetLockState)(static_cast<int>(value));
        }
    }
}
