#pragma once

namespace Unity
{
    struct Scene
    {
        int m_Handle = 0;
    };

    enum class LoadSceneMode : int
    {
        Single = 0,
        Additive = 1
    };

    struct LoadSceneParameters
    {
        LoadSceneMode m_Mode;

        LoadSceneParameters(LoadSceneMode mode = LoadSceneMode::Single)
            : m_Mode(mode) {
        }
    };

    struct SceneManagerFunctions_t
    {
        // properties
        void* m_GetSceneCount = nullptr;

        // active scene
        void* m_GetActiveScene = nullptr;
        void* m_SetActiveScene = nullptr;

        // get scenes
        void* m_GetSceneAt = nullptr;
        void* m_GetSceneByName = nullptr;
        void* m_GetSceneByPath = nullptr;
        void* m_GetSceneByBuildIndex = nullptr;

        // LoadScene overloads
        void* m_LoadScene_Name = nullptr;         // (string) -> Scene
        void* m_LoadScene_Index = nullptr;        // (int) -> Scene
        void* m_LoadScene_Name_Mode = nullptr;    // (string, LoadSceneMode) -> Scene
        void* m_LoadScene_Index_Mode = nullptr;   // (int, LoadSceneMode) -> Scene

        // LoadSceneAsync overloads -> AsyncOperation
        void* m_LoadSceneAsync_Name = nullptr;       // (string)
        void* m_LoadSceneAsync_Index = nullptr;      // (int)
        void* m_LoadSceneAsync_Name_Mode = nullptr;  // (string, LoadSceneMode)
        void* m_LoadSceneAsync_Index_Mode = nullptr; // (int, LoadSceneMode)

        // UnloadSceneAsync overloads -> AsyncOperation
        void* m_UnloadSceneAsync_Scene = nullptr;  // (Scene)
        void* m_UnloadSceneAsync_Name = nullptr;   // (string)
        void* m_UnloadSceneAsync_Index = nullptr;  // (int)

        // misc
        void* m_MergeScenes = nullptr;               // (Scene, Scene)
        void* m_MoveGameObjectToScene = nullptr;     // (GameObject, Scene)
    };

    inline SceneManagerFunctions_t m_SceneManagerFunctions;

    namespace SceneManager
    {
        inline void Initialize()
        {
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_SCENEMANAGER_CLASS);
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_SCENE_CLASS);
            IL2CPP::SystemTypeCache::Initializer::Add(UNITY_ASYNCOP_CLASS);

            auto resolveStatic = [&](void*& outPtr,
                const char* methodName, int argCount,
                std::initializer_list<const char*> icallNames)
                {
                    outPtr = nullptr;

                    // 1) managed method
                    if (void* p = IL2CPP::ResolveUnityMethod(UNITY_SCENEMANAGER_CLASS, methodName, argCount))
                    {
                        outPtr = p; return;
                    }

                    // 2) icall fallback
                    if (void* p = IL2CPP::ResolveCallAny(icallNames))
                    {
                        outPtr = p; return;
                    }
                };

            resolveStatic(m_SceneManagerFunctions.m_GetSceneCount,
                "get_sceneCount", 0,
                { UNITY_SM_SCENECOUNT, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::get_sceneCount") });

            resolveStatic(m_SceneManagerFunctions.m_GetActiveScene,
                "GetActiveScene", 0,
                { UNITY_SM_GETACTIVESCENE, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetActiveScene") });

            resolveStatic(m_SceneManagerFunctions.m_SetActiveScene,
                "SetActiveScene", 1,
                { UNITY_SM_SETACTIVESCENE, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::SetActiveScene") });

            resolveStatic(m_SceneManagerFunctions.m_GetSceneAt,
                "GetSceneAt", 1,
                { UNITY_SM_GETSCENEAT, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneAt") });

            resolveStatic(m_SceneManagerFunctions.m_GetSceneByName,
                "GetSceneByName", 1,
                { UNITY_SM_GETSCENEBYNAME, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneByName") });

            resolveStatic(m_SceneManagerFunctions.m_GetSceneByPath,
                "GetSceneByPath", 1,
                { UNITY_SM_GETSCENEBYPATH, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneByPath") });

            resolveStatic(m_SceneManagerFunctions.m_GetSceneByBuildIndex,
                "GetSceneByBuildIndex", 1,
                { UNITY_SM_GETSCENEBYBUILDINDEX, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneByBuildIndex") });

            // LoadScene
            resolveStatic(m_SceneManagerFunctions.m_LoadScene_Name,
                "LoadScene", 1,
                { UNITY_SM_LOADSCENE_NAME, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadScene") });

            resolveStatic(m_SceneManagerFunctions.m_LoadScene_Index,
                "LoadScene", 1,
                { UNITY_SM_LOADSCENE_INDEX });

            resolveStatic(m_SceneManagerFunctions.m_LoadScene_Name_Mode,
                "LoadScene", 2,
                { UNITY_SM_LOADSCENE_NAME_MODE });

            resolveStatic(m_SceneManagerFunctions.m_LoadScene_Index_Mode,
                "LoadScene", 2,
                { UNITY_SM_LOADSCENE_INDEX_MODE });

            // LoadSceneAsync
            resolveStatic(m_SceneManagerFunctions.m_LoadSceneAsync_Name,
                "LoadSceneAsync", 1,
                { UNITY_SM_LOADSCENEASYNC_NAME, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadSceneAsync") });

            resolveStatic(m_SceneManagerFunctions.m_LoadSceneAsync_Index,
                "LoadSceneAsync", 1,
                { UNITY_SM_LOADSCENEASYNC_INDEX });

            resolveStatic(m_SceneManagerFunctions.m_LoadSceneAsync_Name_Mode,
                "LoadSceneAsync", 2,
                { UNITY_SM_LOADSCENEASYNC_NAME_MODE });

            resolveStatic(m_SceneManagerFunctions.m_LoadSceneAsync_Index_Mode,
                "LoadSceneAsync", 2,
                { UNITY_SM_LOADSCENEASYNC_INDEX_MODE });

            // UnloadSceneAsync
            resolveStatic(m_SceneManagerFunctions.m_UnloadSceneAsync_Scene,
                "UnloadSceneAsync", 1,
                { UNITY_SM_UNLOADSCENEASYNC_SCENE, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::UnloadSceneAsync") });

            resolveStatic(m_SceneManagerFunctions.m_UnloadSceneAsync_Name,
                "UnloadSceneAsync", 1,
                { UNITY_SM_UNLOADSCENEASYNC_NAME });

            resolveStatic(m_SceneManagerFunctions.m_UnloadSceneAsync_Index,
                "UnloadSceneAsync", 1,
                { UNITY_SM_UNLOADSCENEASYNC_INDEX });

            // misc
            resolveStatic(m_SceneManagerFunctions.m_MergeScenes,
                "MergeScenes", 2,
                { UNITY_SM_MERGESCENES, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::MergeScenes") });

            resolveStatic(m_SceneManagerFunctions.m_MoveGameObjectToScene,
                "MoveGameObjectToScene", 2,
                { UNITY_SM_MOVEGAMEOBJECTTOSCENE, IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::MoveGameObjectToScene") });
        }

        // ------------- Public API -------------

        inline int GetSceneCount()
        {
            return reinterpret_cast<int(UNITY_CALLING_CONVENTION)()>(
                m_SceneManagerFunctions.m_GetSceneCount)();
        }

        inline Scene GetActiveScene()
        {
            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)()>(
                m_SceneManagerFunctions.m_GetActiveScene)();
        }

        inline bool SetActiveScene(Scene s)
        {
            return reinterpret_cast<bool(UNITY_CALLING_CONVENTION)(Scene)>(
                m_SceneManagerFunctions.m_SetActiveScene)(s);
        }

        inline Scene GetSceneAt(int index)
        {
            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(int)>(
                m_SceneManagerFunctions.m_GetSceneAt)(index);
        }

        inline Scene GetSceneByName(const char* name)
        {
            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(System_String*)>(
                m_SceneManagerFunctions.m_GetSceneByName)(IL2CPP::String::New(name));
        }

        inline Scene GetSceneByPath(const char* path)
        {
            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(System_String*)>(
                m_SceneManagerFunctions.m_GetSceneByPath)(IL2CPP::String::New(path));
        }

        inline Scene GetSceneByBuildIndex(int buildIndex)
        {
            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(int)>(
                m_SceneManagerFunctions.m_GetSceneByBuildIndex)(buildIndex);
        }

        // LoadScene returns Scene in Unity API
        inline Scene LoadScene(const char* sceneName, LoadSceneMode mode = LoadSceneMode::Single)
        {
            if (mode == LoadSceneMode::Single && m_SceneManagerFunctions.m_LoadScene_Name)
            {
                return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(System_String*)>(
                    m_SceneManagerFunctions.m_LoadScene_Name)(IL2CPP::String::New(sceneName));
            }

            // mode overload
            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(System_String*, LoadSceneMode)>(
                m_SceneManagerFunctions.m_LoadScene_Name_Mode)(IL2CPP::String::New(sceneName), mode);
        }

        inline Scene LoadScene(int buildIndex, LoadSceneMode mode = LoadSceneMode::Single)
        {
            if (mode == LoadSceneMode::Single && m_SceneManagerFunctions.m_LoadScene_Index)
            {
                return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(int)>(
                    m_SceneManagerFunctions.m_LoadScene_Index)(buildIndex);
            }

            return reinterpret_cast<Scene(UNITY_CALLING_CONVENTION)(int, LoadSceneMode)>(
                m_SceneManagerFunctions.m_LoadScene_Index_Mode)(buildIndex, mode);
        }

        inline il2cppObject* LoadSceneAsync(const char* sceneName, LoadSceneMode mode = LoadSceneMode::Single)
        {
            if (mode == LoadSceneMode::Single && m_SceneManagerFunctions.m_LoadSceneAsync_Name)
            {
                return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(System_String*)>(
                    m_SceneManagerFunctions.m_LoadSceneAsync_Name)(IL2CPP::String::New(sceneName));
            }

            return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(System_String*, LoadSceneMode)>(
                m_SceneManagerFunctions.m_LoadSceneAsync_Name_Mode)(IL2CPP::String::New(sceneName), mode);
        }

        inline il2cppObject* LoadSceneAsync(int buildIndex, LoadSceneMode mode = LoadSceneMode::Single)
        {
            if (mode == LoadSceneMode::Single && m_SceneManagerFunctions.m_LoadSceneAsync_Index)
            {
                return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(int)>(
                    m_SceneManagerFunctions.m_LoadSceneAsync_Index)(buildIndex);
            }

            return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(int, LoadSceneMode)>(
                m_SceneManagerFunctions.m_LoadSceneAsync_Index_Mode)(buildIndex, mode);
        }

        inline il2cppObject* UnloadSceneAsync(Scene s)
        {
            return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(Scene)>(
                m_SceneManagerFunctions.m_UnloadSceneAsync_Scene)(s);
        }

        inline il2cppObject* UnloadSceneAsync(const char* sceneName)
        {
            return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(System_String*)>(
                m_SceneManagerFunctions.m_UnloadSceneAsync_Name)(IL2CPP::String::New(sceneName));
        }

        inline il2cppObject* UnloadSceneAsync(int buildIndex)
        {
            return reinterpret_cast<il2cppObject * (UNITY_CALLING_CONVENTION)(int)>(
                m_SceneManagerFunctions.m_UnloadSceneAsync_Index)(buildIndex);
        }

        inline void MergeScenes(Scene sourceScene, Scene destinationScene)
        {
            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(Scene, Scene)>(
                m_SceneManagerFunctions.m_MergeScenes)(sourceScene, destinationScene);
        }

        inline void MoveGameObjectToScene(CGameObject* go, Scene scene)
        {
            reinterpret_cast<void(UNITY_CALLING_CONVENTION)(CGameObject*, Scene)>(
                m_SceneManagerFunctions.m_MoveGameObjectToScene)(go, scene);
        }
    }
}
