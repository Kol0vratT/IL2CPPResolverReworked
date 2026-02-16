#pragma once

namespace Unity
{
    struct InputFunctions_t
    {
        void* m_GetAxis = nullptr;
        void* m_GetAxisRaw = nullptr;

        void* m_GetMouseButton = nullptr;
        void* m_GetMouseButtonDown = nullptr;
        void* m_GetMouseButtonUp = nullptr;

        void* m_GetMousePosition_Injected = nullptr;
        void* m_GetMousePosition_Value = nullptr;

        void* m_GetTouchCount = nullptr;
    };
    inline InputFunctions_t m_InputFunctions;

    namespace Input
    {
        inline void Initialize()
        {
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_INPUT_CLASS);

            auto resolveStatic = [&](void*& outPtr,
                const char* methodName, int argCount,
                std::initializer_list<const char*> icallNames)
                {
                    outPtr = nullptr;

                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_INPUT_CLASS, methodName, argCount))
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

            resolveStatic(m_InputFunctions.m_GetAxis,
                "GetAxis", 1,
                { UNITY_INPUT_GETAXIS, IL2CPP_RStr(UNITY_INPUT_CLASS"::GetAxis(System.String)") });

            resolveStatic(m_InputFunctions.m_GetAxisRaw,
                "GetAxisRaw", 1,
                { UNITY_INPUT_GETAXISRAW, IL2CPP_RStr(UNITY_INPUT_CLASS"::GetAxisRaw(System.String)") });

            resolveStatic(m_InputFunctions.m_GetMouseButton,
                "GetMouseButton", 1,
                { UNITY_INPUT_GETMOUSEBUTTON, IL2CPP_RStr(UNITY_INPUT_CLASS"::GetMouseButton(System.Int32)") });

            resolveStatic(m_InputFunctions.m_GetMouseButtonDown,
                "GetMouseButtonDown", 1,
                { UNITY_INPUT_GETMOUSEBUTTONDOWN, IL2CPP_RStr(UNITY_INPUT_CLASS"::GetMouseButtonDown(System.Int32)") });

            resolveStatic(m_InputFunctions.m_GetMouseButtonUp,
                "GetMouseButtonUp", 1,
                { UNITY_INPUT_GETMOUSEBUTTONUP, IL2CPP_RStr(UNITY_INPUT_CLASS"::GetMouseButtonUp(System.Int32)") });

            // mousePosition: prefer injected(out), fallback to value-return property getter.
            resolveStatic(m_InputFunctions.m_GetMousePosition_Injected,
                "get_mousePosition_Injected", 1,
                {
                    UNITY_INPUT_GETMOUSEPOSITION_INJECTED,
                    IL2CPP_RStr(UNITY_INPUT_CLASS"::get_mousePosition_Injected(UnityEngine.Vector3&)"),
                });

            resolveStatic(m_InputFunctions.m_GetMousePosition_Value,
                "get_mousePosition", 0,
                { UNITY_INPUT_GETMOUSEPOSITION, IL2CPP_RStr(UNITY_INPUT_CLASS"::get_mousePosition") });

            resolveStatic(m_InputFunctions.m_GetTouchCount,
                "get_touchCount", 0,
                { UNITY_INPUT_GETTOUCHCOUNT, IL2CPP_RStr(UNITY_INPUT_CLASS"::get_touchCount") });
        }

        inline float GetAxis(const char* axisName)
        {
            if (!m_InputFunctions.m_GetAxis || !axisName)
                return 0.f;

            System_String* axis = IL2CPP::String::New(axisName);
            if (!axis)
                return 0.f;

            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(System_String*)>(
                m_InputFunctions.m_GetAxis)(axis);
        }

        inline float GetAxisRaw(const char* axisName)
        {
            if (!m_InputFunctions.m_GetAxisRaw || !axisName)
                return 0.f;

            System_String* axis = IL2CPP::String::New(axisName);
            if (!axis)
                return 0.f;

            return reinterpret_cast<float(UNITY_CALLING_CONVENTION)(System_String*)>(
                m_InputFunctions.m_GetAxisRaw)(axis);
        }

        inline bool GetMouseButton(int button)
        {
            if (!m_InputFunctions.m_GetMouseButton)
                return false;

            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(int)>(
                m_InputFunctions.m_GetMouseButton)(button);
        }

        inline bool GetMouseButtonDown(int button)
        {
            if (!m_InputFunctions.m_GetMouseButtonDown)
                return false;

            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(int)>(
                m_InputFunctions.m_GetMouseButtonDown)(button);
        }

        inline bool GetMouseButtonUp(int button)
        {
            if (!m_InputFunctions.m_GetMouseButtonUp)
                return false;

            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(int)>(
                m_InputFunctions.m_GetMouseButtonUp)(button);
        }

        inline Vector3 GetMousePosition()
        {
            if (m_InputFunctions.m_GetMousePosition_Injected)
            {
                Vector3 v{};
                reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Vector3&)>(
                    m_InputFunctions.m_GetMousePosition_Injected)(v);
                return v;
            }

            if (m_InputFunctions.m_GetMousePosition_Value)
            {
                return reinterpret_cast<Vector3(UNITY_CALLING_CONVENTION)()>(
                    m_InputFunctions.m_GetMousePosition_Value)();
            }

            return {};
        }

        inline int GetTouchCount()
        {
            if (!m_InputFunctions.m_GetTouchCount)
                return 0;

            return reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
                m_InputFunctions.m_GetTouchCount)();
        }
    }
}
