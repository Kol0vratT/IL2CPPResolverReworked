#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Optional overrides for specific targets:
// #define IL2CPP_MAIN_MODULE "GameAssembly.dll"
// #define UNITY_VERSION_PRE_2022_3_8F1

#include <Windows.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#include "IL2CPP_Resolver.hpp"

namespace
{
    HMODULE g_Module = nullptr;
    bool g_OwnsConsole = false;
    volatile long g_UpdateCount = 0;
    volatile long g_LateUpdateCount = 0;
    volatile long g_MainThreadSmokeState = 0;
    unsigned long g_MainThreadSmokeException = 0;

    void Log(const char* format, ...);
    void LogSection(const char* name);
    void RunMainThreadUnityApiTests();

    void OnSampleLateUpdate()
    {
        InterlockedIncrement(&g_LateUpdateCount);
    }

    void OnSampleUpdate()
    {
        InterlockedIncrement(&g_UpdateCount);

        if (InterlockedCompareExchange(&g_MainThreadSmokeState, 2, 1) != 1)
            return;

        LogSection("Main Thread Smoke");
        Log("Unity API smoke tests are running on the Unity main thread.");

        __try
        {
            RunMainThreadUnityApiTests();
            InterlockedExchange(&g_MainThreadSmokeState, 3);
            Log("Unity API smoke tests finished.");
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            g_MainThreadSmokeException = static_cast<unsigned long>(GetExceptionCode());
            InterlockedExchange(&g_MainThreadSmokeState, -1);
            Log("Unity API smoke tests crashed with SEH 0x%08lX.", g_MainThreadSmokeException);
        }
    }

    const char* BoolText(bool value)
    {
        return value ? "true" : "false";
    }

    bool SetupConsole()
    {
        if (!GetConsoleWindow())
        {
            if (!AllocConsole())
                return false;

            g_OwnsConsole = true;
        }

        SetConsoleTitleA("IL2CPP Resolver Sample DLL");
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        FILE* stream = nullptr;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
        freopen_s(&stream, "CONIN$", "r", stdin);
        return true;
    }

    void TeardownConsole()
    {
        if (g_OwnsConsole)
            FreeConsole();

        g_OwnsConsole = false;
    }

    void Log(const char* format, ...)
    {
        SYSTEMTIME st{};
        GetLocalTime(&st);

        std::printf("[%02u:%02u:%02u] ",
            static_cast<unsigned>(st.wHour),
            static_cast<unsigned>(st.wMinute),
            static_cast<unsigned>(st.wSecond));

        va_list args;
        va_start(args, format);
        std::vprintf(format, args);
        va_end(args);

        std::printf("\n");
        std::fflush(stdout);
    }

    void LogSection(const char* name)
    {
        Log("=== %s ===", name ? name : "Section");
    }

    void LogResolve(const char* name, const void* address)
    {
        Log("[resolve] %-42s : %s (%p)",
            name ? name : "<null>",
            address ? "ok" : "missing",
            address);
    }

    void LogResolveOptional(const char* name, const void* address)
    {
        Log("[optional] %-40s : %s (%p)",
            name ? name : "<null>",
            address ? "ok" : "missing / possibly stripped",
            address);
    }

    void LogMultiline(const std::string& text)
    {
        if (text.empty())
        {
            Log("<empty>");
            return;
        }

        size_t start = 0;
        while (start <= text.size())
        {
            const size_t end = text.find('\n', start);
            const std::string line = end == std::string::npos
                ? text.substr(start)
                : text.substr(start, end - start);

            Log("%s", line.c_str());

            if (end == std::string::npos)
                break;

            start = end + 1;
        }
    }

    std::string SafeString(Unity::System_String* value)
    {
        return value ? value->ToString() : std::string("<null>");
    }

    std::string SafeObjectName(Unity::CObject* object)
    {
        if (!object)
            return "<null>";

        if (Unity::System_String* name = object->GetName())
            return name->ToString();

        Unity::il2cppObject* managed = object->GetManagedObject();
        if (managed && managed->m_pClass && managed->m_pClass->m_pName)
            return std::string("<") + managed->m_pClass->m_pName + ">";

        return "<unnamed>";
    }

    std::string ToString(const Unity::Vector3& value)
    {
        char buffer[96]{};
        std::snprintf(buffer, sizeof(buffer), "(%.3f, %.3f, %.3f)", value.x, value.y, value.z);
        return std::string(buffer);
    }

    std::string ToString(const Unity::Quaternion& value)
    {
        char buffer[128]{};
        std::snprintf(buffer, sizeof(buffer), "(%.3f, %.3f, %.3f, %.3f)", value.x, value.y, value.z, value.w);
        return std::string(buffer);
    }

    std::string ToString(const Unity::Color& value)
    {
        char buffer[128]{};
        std::snprintf(buffer, sizeof(buffer), "(%.3f, %.3f, %.3f, %.3f)", value.r, value.g, value.b, value.a);
        return std::string(buffer);
    }

    template <typename T>
    unsigned long long ArrayCount(const Unity::il2cppArray<T>* value)
    {
        return value ? static_cast<unsigned long long>(value->Size()) : 0ULL;
    }

    void TestCore()
    {
        LogSection("Core / IL2CPP");

        void* domain = IL2CPP::Domain::Get();
        size_t assemblyCount = 0;
        Unity::il2cppAssembly** assemblies = IL2CPP::Domain::GetAssemblies(&assemblyCount);

        Unity::il2cppClass* stringClass = IL2CPP::Class::Find("System.String");
        Unity::il2cppClass* cameraClass = IL2CPP::Class::Find(UNITY_CAMERA_CLASS);
        Unity::il2cppObject* cameraType = IL2CPP::Class::GetSystemType(cameraClass);
        Unity::il2cppObject* cachedCameraType = IL2CPP::SystemTypeCache::Get(UNITY_CAMERA_CLASS);

        Unity::System_String* managedString = IL2CPP::String::New("IL2CPP Resolver sample");
        Unity::System_String* noGcString = IL2CPP::String::NoGC::New("IL2CPP Resolver sample (NoGC)");

        Log("Domain                       : %p", domain);
        Log("Assemblies                   : %p, count=%llu", assemblies, static_cast<unsigned long long>(assemblyCount));
        Log("Class::Find(System.String)   : %p", stringClass);
        Log("Class::Find(Camera)          : %p", cameraClass);
        Log("Class::GetSystemType(Camera) : %p", cameraType);
        Log("SystemTypeCache::Get(Camera) : %p", cachedCameraType);
        Log("String::New                  : %s", SafeString(managedString).c_str());
        Log("String::NoGC::New            : %s", SafeString(noGcString).c_str());

        LogResolve("ResolveUnityMethod(Time::get_timeScale)",
            IL2CPP::ResolveUnityMethod(UNITY_TIME_CLASS, "get_timeScale", 0));
        LogResolve("ResolveUnityMethod(PlayerPrefs::GetInt)",
            IL2CPP::ResolveUnityMethod(UNITY_PLAYERPREFS_CLASS, "GetInt", { "System.String", "System.Int32" }));
        LogResolve("ResolveCallCached(Time::get_timeScale)",
            IL2CPP::ResolveCallCached(UNITY_TIME_GET_TIMESCALE));
        LogResolve("ResolveUnityMethodOrIcall(Debug::Log)",
            IL2CPP::ResolveUnityMethodOrIcall(UNITY_DEBUG_CLASS, "Log", { "System.String" }, { UNITY_DEBUG_LOG_STR }));
    }

    void TestApplication()
    {
        LogSection("Application");

        const int targetFrameRate = Unity::Application::GetTargetFrameRate();
        Unity::Application::SetTargetFrameRate(targetFrameRate);

        Log("Unity version      : %s", SafeString(Unity::Application::GetUnityVersion()).c_str());
        Log("Product name       : %s", SafeString(Unity::Application::GetProductName()).c_str());
        Log("Identifier         : %s", SafeString(Unity::Application::GetIdentifier()).c_str());
        Log("Data path          : %s", SafeString(Unity::Application::GetDataPath()).c_str());
        Log("Persistent path    : %s", SafeString(Unity::Application::GetPersistentDataPath()).c_str());
        Log("Streaming path     : %s", SafeString(Unity::Application::GetStreamingAssetsPath()).c_str());
        Log("Focused            : %s", BoolText(Unity::Application::GetIsFocused()));
        Log("Target frame rate  : %d", targetFrameRate);
        LogResolve("Application::Quit", Unity::m_ApplicationFunctions.m_Quit);
    }

    void TestScreen()
    {
        LogSection("Screen");

        const bool fullScreen = Unity::Screen::GetFullScreen();
        Unity::Screen::SetFullScreen(fullScreen);

        Log("Width       : %d", Unity::Screen::GetWidth());
        Log("Height      : %d", Unity::Screen::GetHeight());
        Log("DPI         : %.2f", Unity::Screen::GetDpi());
        Log("FullScreen  : %s", BoolText(fullScreen));
    }

    void TestTime()
    {
        LogSection("Time");

        const float fixedDeltaTime = Unity::Time::GetFixedDeltaTime();
        const float timeScale = Unity::Time::GetTimeScale();

        Unity::Time::SetFixedDeltaTime(fixedDeltaTime);
        Unity::Time::SetTimeScale(timeScale);

        Log("deltaTime         : %.6f", Unity::Time::GetDeltaTime());
        Log("unscaledDeltaTime : %.6f", Unity::Time::GetUnscaledDeltaTime());
        Log("time              : %.6f", Unity::Time::GetTime());
        Log("unscaledTime      : %.6f", Unity::Time::GetUnscaledTime());
        Log("fixedDeltaTime    : %.6f", fixedDeltaTime);
        Log("timeScale         : %.6f", timeScale);
        Log("frameCount        : %d", Unity::Time::GetFrameCount());
    }

    void TestCursor()
    {
        LogSection("Cursor");

        const bool visible = Unity::Cursor::GetVisible();
        const Unity::Cursor::m_eLockMode lockState = Unity::Cursor::GetLockState();

        Unity::Cursor::SetVisible(visible);
        Unity::Cursor::SetLockState(lockState);

        Log("Visible    : %s", BoolText(visible));
        Log("Lock state : %d", static_cast<int>(lockState));
    }

    void TestInput()
    {
        LogSection("Input");

        const Unity::Vector3 mousePosition = Unity::Input::GetMousePosition();

        Log("Axis(Horizontal)      : %.3f", Unity::Input::GetAxis("Horizontal"));
        Log("AxisRaw(Horizontal)   : %.3f", Unity::Input::GetAxisRaw("Horizontal"));
        Log("MouseButton(0)        : %s", BoolText(Unity::Input::GetMouseButton(0)));
        Log("MouseButtonDown(0)    : %s", BoolText(Unity::Input::GetMouseButtonDown(0)));
        Log("MouseButtonUp(0)      : %s", BoolText(Unity::Input::GetMouseButtonUp(0)));
        Log("Mouse position        : %s", ToString(mousePosition).c_str());
        Log("Touch count           : %d", Unity::Input::GetTouchCount());
    }

    void TestLayerMask()
    {
        LogSection("LayerMask");

        const uint32_t defaultLayer = Unity::LayerMask::NameToLayer("Default");
        Log("NameToLayer(Default)  : %u", static_cast<unsigned>(defaultLayer));
        if (Unity::m_LayerMaskFunctions.m_LayerToName)
            Log("LayerToName(0)        : %s", SafeString(Unity::LayerMask::LayerToName(0)).c_str());
        else
            Log("LayerToName(0)        : <unavailable>");
        LogResolve("LayerMask::NameToLayer", Unity::m_LayerMaskFunctions.m_NameToLayer);
        LogResolveOptional("LayerMask::LayerToName", Unity::m_LayerMaskFunctions.m_LayerToName);
    }

    void TestDebug()
    {
        LogSection("Debug");

        Unity::Debug::Log("[IL2CPP Resolver Sample] Debug::Log");
        Unity::Debug::LogWarning("[IL2CPP Resolver Sample] Debug::LogWarning");
        Unity::Debug::LogError("[IL2CPP Resolver Sample] Debug::LogError");

        Log("Debug messages sent to Unity log.");
    }

    void TestRenderSettings()
    {
        LogSection("RenderSettings");

        const bool fog = Unity::RenderSettings::GetFog();
        const Unity::Color fogColor = Unity::RenderSettings::GetFogColor();
        const Unity::Color ambientLight = Unity::RenderSettings::GetAmbientLight();
        Unity::CObject* skybox = Unity::RenderSettings::GetSkybox();
        Unity::CObject* sun = Unity::RenderSettings::GetSun();

        Unity::RenderSettings::SetFog(fog);
        Unity::RenderSettings::SetFogColor(fogColor);
        Unity::RenderSettings::SetAmbientLight(ambientLight);
        Unity::RenderSettings::SetSkybox(skybox);
        Unity::RenderSettings::SetSun(sun);

        Log("Fog           : %s", BoolText(fog));
        Log("Fog color     : %s", ToString(fogColor).c_str());
        Log("Ambient light : %s", ToString(ambientLight).c_str());
        Log("Skybox        : %s", SafeObjectName(skybox).c_str());
        Log("Sun           : %s", SafeObjectName(sun).c_str());
    }

    void TestPlayerPrefs()
    {
        LogSection("PlayerPrefs");

        char keyInt[96]{};
        char keyFloat[96]{};
        char keyString[96]{};

        std::snprintf(keyInt, sizeof(keyInt), "IL2CPP_Resolver_Sample_Int_%lu", static_cast<unsigned long>(GetCurrentProcessId()));
        std::snprintf(keyFloat, sizeof(keyFloat), "IL2CPP_Resolver_Sample_Float_%lu", static_cast<unsigned long>(GetCurrentProcessId()));
        std::snprintf(keyString, sizeof(keyString), "IL2CPP_Resolver_Sample_String_%lu", static_cast<unsigned long>(GetCurrentProcessId()));

        const bool hasFloatGet = Unity::m_PlayerPrefsFunctions.m_GetFloat1 != nullptr || Unity::m_PlayerPrefsFunctions.m_GetFloat2 != nullptr;
        const bool hasFloatSet = Unity::m_PlayerPrefsFunctions.m_SetFloat != nullptr;

        Unity::PlayerPrefs::SetInt(keyInt, 1337);
        if (hasFloatSet)
            Unity::PlayerPrefs::SetFloat(keyFloat, 3.5f);
        Unity::PlayerPrefs::SetString(keyString, "sample-value");
        Unity::PlayerPrefs::Save();

        Log("GetInt        : %d", Unity::PlayerPrefs::GetInt(keyInt, -1));
        if (hasFloatGet)
            Log("GetFloat      : %.3f", Unity::PlayerPrefs::GetFloat(keyFloat, -1.0f));
        else
            Log("GetFloat      : <unavailable>");
        Log("GetString     : %s", Unity::PlayerPrefs::GetStringUtf8(keyString, "<missing>").c_str());
        Log("HasKey(Int)   : %s", BoolText(Unity::PlayerPrefs::HasKey(keyInt)));
        if (hasFloatSet)
            Log("HasKey(Float) : %s", BoolText(Unity::PlayerPrefs::HasKey(keyFloat)));
        else
            Log("HasKey(Float) : <skipped>");
        Log("HasKey(Str)   : %s", BoolText(Unity::PlayerPrefs::HasKey(keyString)));
        LogResolveOptional("PlayerPrefs::GetFloat(default)", Unity::m_PlayerPrefsFunctions.m_GetFloat2);
        LogResolveOptional("PlayerPrefs::SetFloat", Unity::m_PlayerPrefsFunctions.m_SetFloat);

        Unity::PlayerPrefs::DeleteKey(keyInt);
        Unity::PlayerPrefs::DeleteKey(keyFloat);
        Unity::PlayerPrefs::DeleteKey(keyString);
        Unity::PlayerPrefs::Save();

        Log("After delete   : int=%s, float=%s, string=%s",
            BoolText(Unity::PlayerPrefs::HasKey(keyInt)),
            hasFloatSet ? BoolText(Unity::PlayerPrefs::HasKey(keyFloat)) : "<skipped>",
            BoolText(Unity::PlayerPrefs::HasKey(keyString)));

        LogResolve("PlayerPrefs::DeleteAll", Unity::m_PlayerPrefsFunctions.m_DeleteAll);
    }

    void TestResources()
    {
        LogSection("Resources");

        Unity::il2cppArray<Unity::CCamera*>* cameras = Unity::Resources::FindObjectsOfTypeAll<Unity::CCamera>(UNITY_CAMERA_CLASS);

        Log("FindObjectsOfTypeAll(Camera) : count=%llu", ArrayCount(cameras));
        LogResolve("Resources::Load(path)", Unity::m_ResourcesFunctions.m_Load1);
        LogResolve("Resources::Load(path, type)", Unity::m_ResourcesFunctions.m_Load2);
        LogResolve("Resources::UnloadUnusedAssets", Unity::m_ResourcesFunctions.m_UnloadUnusedAssets);
    }

    void TestSceneManager(Unity::CGameObject* sampleObject)
    {
        LogSection("SceneManager");

        const int sceneCount = Unity::SceneManager::GetSceneCount();
        const Unity::Scene activeScene = Unity::SceneManager::GetActiveScene();

        Log("Scene count        : %d", sceneCount);
        Log("Active scene handle: %d", activeScene.m_Handle);

        if (sceneCount > 0)
        {
            const Unity::Scene sceneAtZero = Unity::SceneManager::GetSceneAt(0);
            Log("Scene[0] handle    : %d", sceneAtZero.m_Handle);
        }

        Log("BuildIndex(0)      : %d", Unity::SceneManager::GetSceneByBuildIndex(0).m_Handle);

        if (sampleObject)
        {
            LogResolve("SceneManager::MoveGameObjectToScene", Unity::m_SceneManagerFunctions.m_MoveGameObjectToScene);
        }

        LogResolve("SceneManager::SetActiveScene", Unity::m_SceneManagerFunctions.m_SetActiveScene);
        LogResolve("SceneManager::GetSceneByName", Unity::m_SceneManagerFunctions.m_GetSceneByName);
        LogResolve("SceneManager::GetSceneByPath", Unity::m_SceneManagerFunctions.m_GetSceneByPath);
        LogResolve("SceneManager::LoadScene(name)", Unity::m_SceneManagerFunctions.m_LoadScene_Name);
        LogResolveOptional("SceneManager::LoadScene(index)", Unity::m_SceneManagerFunctions.m_LoadScene_Index);
        LogResolve("SceneManager::LoadSceneAsync(name)", Unity::m_SceneManagerFunctions.m_LoadSceneAsync_Name);
        LogResolve("SceneManager::UnloadSceneAsync(scene)", Unity::m_SceneManagerFunctions.m_UnloadSceneAsync_Scene);
        LogResolveOptional("SceneManager::MergeScenes", Unity::m_SceneManagerFunctions.m_MergeScenes);
    }

    void TestObjectGraph()
    {
        LogSection("Object / GameObject / Camera / Component / Behaviour / Transform / Rigidbody");

        Unity::il2cppArray<Unity::CCamera*>* allCameras = Unity::Object::FindObjectsOfType<Unity::CCamera>(UNITY_CAMERA_CLASS, true);
        Unity::CCamera* mainCamera = Unity::Camera::GetMain();
        Unity::CCamera* currentCamera = Unity::Camera::GetCurrent();
        Unity::CComponent* monoBehaviour = IL2CPP::Helper::GetMonoBehaviour();
        Unity::CRigidbody* rigidbody = Unity::Object::FindObjectOfType<Unity::CRigidbody>(UNITY_RIGIDBODY_CLASS, true);
        Unity::CGameObject* sampleObject = nullptr;
        Unity::CTransform* chosenTransform = nullptr;

        Log("Object::FindObjectsOfType(Camera) : count=%llu", ArrayCount(allCameras));
        Log("Camera::GetMain                   : %s (%p)", SafeObjectName(mainCamera).c_str(), mainCamera);
        Log("Camera::GetCurrent                : %s (%p)", SafeObjectName(currentCamera).c_str(), currentCamera);
        Log("Helper::GetMonoBehaviour          : %p", monoBehaviour);
        Log("Object::FindObjectOfType(Rigidbody): %p", rigidbody);
        LogResolveOptional("GameObject::CreatePrimitive", Unity::m_GameObjectFunctions.m_CreatePrimitive);
        LogResolve("GameObject::AddComponent", Unity::m_GameObjectFunctions.m_AddComponent);
        LogResolve("Object::Destroy", Unity::m_ObjectFunctions.m_Destroy);

        if (mainCamera)
        {
            const float depth = mainCamera->GetDepth();
            const float fov = mainCamera->GetFieldOfView();
            Unity::CGameObject* cameraOwner = mainCamera->GetGameObject();

            mainCamera->SetDepth(depth);
            mainCamera->SetFieldOfView(fov);

            chosenTransform = mainCamera->GetTransform();
            sampleObject = cameraOwner;

            Log("Main camera depth                : %.3f", depth);
            Log("Main camera FOV                  : %.3f", fov);
            Log("Main camera owner                : %s (%p)", SafeObjectName(cameraOwner).c_str(), cameraOwner);
            LogResolve("Camera::WorldToScreen", Unity::m_CameraFunctions.m_WorldToScreen);
            LogResolve("Transform::GetPosition_Injected", Unity::m_TransformFunctions.m_GetPosition_Injected);
            LogResolve("Transform::GetPosition_Value", Unity::m_TransformFunctions.m_GetPosition_Value);

            if (chosenTransform)
            {
                Log("Attempting Transform::GetPosition on camera transform...");
                const Unity::Vector3 cameraPosition = chosenTransform->GetPosition();
                Log("Camera transform position       : %s", ToString(cameraPosition).c_str());

                Log("Attempting Camera::WorldToScreen...");
                Unity::Vector3 screenPosition{};
                mainCamera->WorldToScreen(const_cast<Unity::Vector3&>(cameraPosition), screenPosition);
                Log("WorldToScreen(cameraPos)        : %s", ToString(screenPosition).c_str());
            }
        }

        if (monoBehaviour)
        {
            Log("Attempting MonoBehaviour owner/transform/behaviour calls...");
            Unity::CGameObject* owner = monoBehaviour->GetGameObject();
            Unity::CTransform* ownerTransform = monoBehaviour->GetTransform();
            Unity::CBehaviour* behaviour = reinterpret_cast<Unity::CBehaviour*>(monoBehaviour);

            Log("MonoBehaviour owner              : %s (%p)", SafeObjectName(owner).c_str(), owner);
            Log("MonoBehaviour transform          : %p", ownerTransform);
            Log("Behaviour::GetEnabled            : %s", BoolText(behaviour->GetEnabled()));
            Log("Behaviour::GetIsActiveAndEnabled : %s", BoolText(behaviour->GetIsActiveAndEnabled()));
            behaviour->SetEnabled(behaviour->GetEnabled());

            if (!sampleObject)
                sampleObject = owner;

            if (!chosenTransform)
                chosenTransform = ownerTransform;
        }

        if (rigidbody)
        {
            Log("Attempting Rigidbody calls...");
            const bool detectCollisions = rigidbody->GetDetectCollisions();
            const Unity::Vector3 velocity = rigidbody->GetVelocity();
            Unity::CGameObject* owner = rigidbody->GetGameObject();

            rigidbody->SetDetectCollisions(detectCollisions);
            rigidbody->SetVelocity(velocity);

            Log("Rigidbody detectCollisions       : %s", BoolText(detectCollisions));
            Log("Rigidbody velocity               : %s", ToString(velocity).c_str());
            Log("Rigidbody owner                  : %s (%p)", SafeObjectName(owner).c_str(), owner);

            if (!sampleObject)
                sampleObject = owner;
        }

        if (sampleObject)
        {
            Log("Attempting GameObject calls on sample object...");
            const bool active = sampleObject->GetActive();
            const unsigned layer = sampleObject->GetLayer();
            Unity::CTransform* transform = sampleObject->GetTransform();
            Unity::CComponent* transformByType = sampleObject->GetComponentByType(UNITY_TRANSFORM_CLASS);
            Unity::CComponent* transformByName = sampleObject->GetComponent("Transform");
            Unity::CComponent* transformInChildren = sampleObject->GetComponentInChildren(UNITY_TRANSFORM_CLASS);
            Unity::il2cppArray<Unity::CComponent*>* components = sampleObject->GetComponents(UNITY_COMPONENT_CLASS);
            Unity::il2cppArray<Unity::CGameObject*>* mainCameraTag = Unity::GameObject::FindWithTag("MainCamera");

            sampleObject->SetActive(active);
            sampleObject->SetLayer(layer);

            Log("Sample object active             : %s", BoolText(active));
            Log("Sample object layer              : %u", layer);
            Log("Sample object transform          : %p", transform);
            Log("GetComponentByType(Transform)    : %p", transformByType);
            Log("GetComponent(\"Transform\")       : %p", transformByName);
            Log("GetComponentInChildren(Transform): %p", transformInChildren);
            Log("GetComponents(Component)         : count=%llu", ArrayCount(components));
            Log("GameObject::FindWithTag(MainCamera): count=%llu", ArrayCount(mainCameraTag));
            LogResolve("GameObject::Find", Unity::m_GameObjectFunctions.m_Find);
            LogResolve("GameObject::GetComponent(Type)", Unity::m_GameObjectFunctions.m_GetComponentByType);
            LogResolve("Object::GetName", Unity::m_ObjectFunctions.m_GetName);

            if (!chosenTransform)
                chosenTransform = transform;
        }

        if (chosenTransform)
        {
            Log("Attempting Transform hierarchy/value calls...");
            const int childCount = chosenTransform->GetChildCount();
            const Unity::Vector3 position = chosenTransform->GetPosition();
            const Unity::Quaternion rotation = chosenTransform->GetRotation();
            const Unity::Vector3 localPosition = chosenTransform->GetLocalPosition();
            const Unity::Vector3 localScale = chosenTransform->GetLocalScale();
            Unity::CTransform* parent = chosenTransform->GetParent();
            Unity::CTransform* root = chosenTransform->GetRoot();
            Unity::CTransform* firstChild = childCount > 0 ? chosenTransform->GetChild(0) : nullptr;

            chosenTransform->SetPosition(position);
            chosenTransform->SetRotation(rotation);
            chosenTransform->SetLocalPosition(localPosition);
            chosenTransform->SetLocalScale(localScale);

            Log("Transform parent                 : %p", parent);
            Log("Transform root                   : %p", root);
            Log("Transform childCount             : %d", childCount);
            Log("Transform child[0]               : %p", firstChild);
            Log("Transform position               : %s", ToString(position).c_str());
            Log("Transform rotation               : %s", ToString(rotation).c_str());
            Log("Transform localPosition          : %s", ToString(localPosition).c_str());
            Log("Transform localScale             : %s", ToString(localScale).c_str());
            LogResolveOptional("Transform::FindChild", Unity::m_TransformFunctions.m_FindChild);
        }

        TestSceneManager(sampleObject);
    }

    void RunMainThreadUnityApiTests()
    {
        TestApplication();
        TestScreen();
        TestTime();
        TestCursor();
        TestInput();
        TestLayerMask();
        TestDebug();
        TestRenderSettings();
        TestPlayerPrefs();
        TestResources();
        TestObjectGraph();
    }

    bool InstallMainThreadSmoke()
    {
        LogSection("Callback / Main Thread");

        InterlockedExchange(&g_UpdateCount, 0);
        InterlockedExchange(&g_LateUpdateCount, 0);
        InterlockedExchange(&g_MainThreadSmokeState, 0);
        g_MainThreadSmokeException = 0;

        IL2CPP::Callback::OnUpdate::Add(reinterpret_cast<void*>(&OnSampleUpdate));
        IL2CPP::Callback::OnLateUpdate::Add(reinterpret_cast<void*>(&OnSampleLateUpdate));
        IL2CPP::Callback::Initialize();

        const bool updateHooked =
            IL2CPP::Callback::OnUpdate::m_CallbackHook.m_VFunc != nullptr &&
            IL2CPP::Callback::OnUpdate::m_CallbackHook.m_Original != nullptr;
        const bool lateUpdateHooked =
            IL2CPP::Callback::OnLateUpdate::m_CallbackHook.m_VFunc != nullptr &&
            IL2CPP::Callback::OnLateUpdate::m_CallbackHook.m_Original != nullptr;

        Log("OnUpdate hook      : %s", BoolText(updateHooked));
        Log("OnLateUpdate hook  : %s", BoolText(lateUpdateHooked));

        if (!updateHooked)
            return false;

        InterlockedExchange(&g_MainThreadSmokeState, 1);
        return true;
    }

    void WaitForMainThreadSmoke(unsigned long timeoutMs = 15000UL)
    {
        Log("Waiting for main-thread Unity API smoke tests...");

        const unsigned long startTick = GetTickCount();
        for (;;)
        {
            const long state = InterlockedCompareExchange(&g_MainThreadSmokeState, 0, 0);
            if (state == 3 || state == -1)
                break;

            if ((GetTickCount() - startTick) >= timeoutMs)
            {
                Log("Main-thread smoke tests timed out after %lu ms.", timeoutMs);
                break;
            }

            Sleep(50);
        }

        Log("OnUpdate count     : %ld", g_UpdateCount);
        Log("OnLateUpdate count : %ld", g_LateUpdateCount);

        const long finalState = InterlockedCompareExchange(&g_MainThreadSmokeState, 0, 0);
        if (finalState == 3)
        {
            Log("Main-thread smoke status: success");
        }
        else if (finalState == -1)
        {
            Log("Main-thread smoke status: crashed, SEH=0x%08lX", g_MainThreadSmokeException);
        }
        else
        {
            Log("Main-thread smoke status: not completed");
        }
    }

    void WaitForUnload()
    {
        LogSection("Unload");
        Log("Press END to unload the sample DLL.");

        for (;;)
        {
            if ((GetAsyncKeyState(VK_END) & 1) != 0)
                break;

            Sleep(50);
        }
    }

    DWORD WINAPI SampleThread(void* context)
    {
        g_Module = static_cast<HMODULE>(context);
        SetupConsole();

        Log("Sample thread started.");
        Log("Waiting for IL2CPP module and initializing resolver...");

        if (!IL2CPP::Initialize(true, 90))
        {
            Log("IL2CPP::Initialize failed: %s", IL2CPP::GetLastInitError());
            LogMultiline(IL2CPP::GetInitializationReport());
            WaitForUnload();
            TeardownConsole();
            FreeLibraryAndExitThread(g_Module, 0);
        }

        Log("IL2CPP initialized against module: %s", IL2CPP::GetLoadedModuleName());
        LogMultiline(IL2CPP::GetInitializationReport());

        {
            IL2CPP::ScopedThreadAttachment attachment;
            Log("Thread attached: %s", BoolText(attachment.IsAttached()));
            TestCore();

            if (InstallMainThreadSmoke())
            {
                WaitForMainThreadSmoke();
            }
            else
            {
                Log("Main-thread callback hook was not installed. Unity API smoke tests were skipped.");
            }

            WaitForUnload();
            IL2CPP::Callback::Uninitialize();
        }

        IL2CPP::Reset();
        Log("Resolver state cleared.");

        Sleep(150);
        TeardownConsole();
        FreeLibraryAndExitThread(g_Module, 0);
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    UNREFERENCED_PARAMETER(reserved);

    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(module);

        HANDLE thread = CreateThread(nullptr, 0, &SampleThread, module, 0, nullptr);
        if (thread)
            CloseHandle(thread);
    }

    return TRUE;
}
