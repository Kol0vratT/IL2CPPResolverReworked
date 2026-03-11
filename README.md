# IL2CPP_Resolver Reworked

`IL2CPP_Resolver Reworked` is a header-first C++ runtime bridge for Unity IL2CPP games.  
It resolves IL2CPP exports, resolves managed methods and icalls, and provides a defensive Unity API layer for common engine classes.

## Recent Rework Update

- Converted the resolver to a correct `header-only` layout with inline state/functions.
- Added reset and diagnostics helpers:
  - `IL2CPP::IsInitialized()`
  - `IL2CPP::GetLoadedModuleName()`
  - `IL2CPP::GetInitializationReport()`
  - `IL2CPP::Reset(bool clearModuleHandle = false)`
- Added thread-safe caches for `System.Type`, class lookup, and icall/method resolution.
- Fixed overload resolution, string handling, arrays/dictionaries access, and math helper issues.
- Reworked Unity wrapper hierarchy so `Camera`, `Behaviour`, `Component`, `GameObject`, `Transform`, and `Rigidbody` better match Unity's runtime model.
- Added wrappers for `Behaviour`, `PlayerPrefs`, and `Resources`.
- Made optional Unity APIs stripping-aware so missing methods are treated as unavailable instead of hard failure.
- Added a minimal DLL sample project in `IL2CPP_Resolver_Sample_DLL/` with console logging and main-thread smoke tests.

## What It Does

- Initializes IL2CPP from a loaded game module (`GameAssembly.dll` by default).
- Resolves required and optional IL2CPP exports with safety checks.
- Supports export-name obfuscation fallback (ROT mode).
- Supports custom export resolver callbacks for protected targets.
- Optionally enables heuristic export-name matching.
- Resolves managed Unity methods first, then falls back to icalls.
- Resolves overloaded managed methods by full parameter signature when needed.
- Exposes reusable C++ wrappers for many UnityEngine APIs.
- Stays link-safe as a true header-only library across multiple translation units.
- Includes reset/report utilities, resolver caches, and scoped IL2CPP thread attachment.
- Adds broad null checks and fallback paths to reduce crash risk across Unity versions.

## Repository Layout (Library Only)

- `IL2CPP_Resolver.hpp`
  - Main include and initialization flow.
  - Export resolver pipeline.
  - Unity API initialization entrypoint.
- `Defines.hpp`
  - Canonical IL2CPP export names and calling convention macros.
- `Data.hpp`
  - Global resolver state (`Globals`, `Functions`).
- `SystemTypeCache.hpp`
  - Fast cache for `System.Type` objects by hash/name.
- `API/`
  - Core IL2CPP utilities: domain, classes, strings, threads, callbacks, method/icall resolution.
- `Unity/API/`
  - UnityEngine wrappers (Application, Behaviour, Camera, Component, Cursor, Debug, GameObject, Input, LayerMask, Object, PlayerPrefs, RenderSettings, Resources, Rigidbody, SceneManager, Screen, Time, Transform).
- `Unity/Structures/`
  - IL2CPP/Unity data structures (`il2cppObject`, `il2cppClass`, arrays, strings, vectors, quaternions, etc.).
- `Utils/`
  - Hashing, vtable patching/search helpers, safe MonoBehaviour resolver helper.
- `IL2CPP_Resolver_Sample_DLL/`
  - Minimal DLL template with console logging and main-thread smoke tests.

## Initialization Pipeline

`IL2CPP::Initialize(bool waitForModule = false, int maxSecondsWait = 60)`:

1. Resolves target module (`IL2CPP_MAIN_MODULE`, then fallback module names).
2. Detects export naming mode (`None`, `ROT`).
3. Resolves required IL2CPP exports.  
   Initialization fails if any required symbol is missing.
4. Resolves optional/version-dependent exports if available.
5. Initializes all Unity API wrappers.
6. Pre-caches queued `System.Type` objects via `SystemTypeCache::Initializer`.
7. Marks the resolver initialized and keeps an export-resolution report for diagnostics.

### Required Exports

- `il2cpp_class_from_name`
- `il2cpp_class_get_fields`
- `il2cpp_class_get_field_from_name`
- `il2cpp_class_get_methods`
- `il2cpp_class_get_method_from_name`
- `il2cpp_class_get_property_from_name`
- `il2cpp_class_get_type`
- `il2cpp_domain_get`
- `il2cpp_domain_get_assemblies`
- `il2cpp_string_new`
- `il2cpp_thread_attach`
- `il2cpp_thread_detach`
- `il2cpp_type_get_object`
- `il2cpp_object_new`

### Optional Exports

- `il2cpp_image_get_class`
- `il2cpp_image_get_class_count`
- `il2cpp_resolve_icall` / `il2cpp_codegen_resolve_icall`
- `il2cpp_free`
- `il2cpp_method_get_param_name`
- `il2cpp_method_get_param`
- `il2cpp_class_from_il2cpp_type` / `il2cpp_class_from_type`
- `il2cpp_field_static_get_value`
- `il2cpp_field_static_set_value`

If initialization fails, inspect:

```cpp
const char* reason = IL2CPP::UnityAPI::GetLastInitError();
std::string report = IL2CPP::GetInitializationReport();
```

You can also query:

- `IL2CPP::IsInitialized()`
- `IL2CPP::GetLoadedModuleName()`
- `IL2CPP::Reset(bool clearModuleHandle = false)`

## Export Resolver Flexibility

You can extend/override symbol resolution:

```cpp
static void* MyExportResolver(HMODULE gameModule, const char* canonicalName)
{
    // Your custom logic (manual table, pattern scan, decrypt name, etc.)
    return nullptr;
}

IL2CPP::SetCustomExportResolver(&MyExportResolver);
IL2CPP::SetHeuristicExportResolution(true); // disabled by default
```

## Core API (`API/`)

### `API/ResolveCall.hpp`

- `IL2CPP::ResolveUnityMethod(className, methodName, argCount)`
- `IL2CPP::ResolveUnityMethod(className, methodName, { "Param.Type", ... })`
- `IL2CPP::ResolveCall(icallName)`
- `IL2CPP::ResolveCallCached(icallName)`
- `IL2CPP::ResolveCallAny({ candidates... })`
- `IL2CPP::ResolveUnityMethodOrIcall(...)`
- `IL2CPP::ClearResolverCaches()`

Design: prefer managed method pointer resolution (usually more stable), then fallback to icalls.

### `API/Domain.hpp`

- `IL2CPP::Domain::Get()`
- `IL2CPP::Domain::GetAssemblies(size_t* outCount)`

### `API/Class.hpp`

- Enumerate fields/methods:
  - `Class::GetFields`, `Class::FetchFields`
  - `Class::GetMethods`, `Class::FetchMethods`
- Resolve classes:
  - `Class::Find("Namespace.Type")`
  - `Class::GetFromName(image, namespace, name)`
  - `Class::FetchClasses(...)` (when class count exports exist)
- Type helpers:
  - `Class::GetType`, `Class::GetSystemType`
- Utility helpers:
  - `Class::Utils::GetFieldOffset`
  - `Class::Utils::GetMethodPointer` (name+argc and typed overloads)
  - `Class::Utils::SetStaticField` / `GetStaticField`
  - `Class::Utils::MethodGetParamName`
  - `Class::Utils::GetMethodParamType`
  - `Class::Utils::ClassFromType`
  - class filtering helpers (`FilterClass`, `FilterClassToMethodPointer`)
- Base wrapper class `IL2CPP::CClass`:
  - generic method calls (`CallMethod`, `CallMethodSafe`)
  - property get/set
  - field/member get/set by name/offset
  - obscured-value helpers (`GetObscuredValue`, `SetObscuredValue`)

### `API/String.hpp`

- `IL2CPP::String::New(const char*)` (managed string)
- `IL2CPP::String::NoGC::New(const char*)` (manual/no-GC object construction)

### `API/Thread.hpp`

- `IL2CPP::Thread::Attach(domain)`
- `IL2CPP::Thread::Detach(thread)`
- `IL2CPP::Thread::Create(startFn, endFn)`  
  Creates native thread and auto attach/detach to IL2CPP domain.
- `IL2CPP::ScopedThreadAttachment`  
  RAII attach/detach helper for the current native thread.

### `API/Callback.hpp`

- Callback hooks for:
  - `IL2CPP::Callback::OnUpdate::Add(fn)`
  - `IL2CPP::Callback::OnLateUpdate::Add(fn)`
- Runtime hook control:
  - `IL2CPP::Callback::Initialize()`
  - `IL2CPP::Callback::Uninitialize()`

Internally uses vtable scanning plus function replacement (`Utils/VTable.hpp`).

### `API/Class.hpp`

- Class lookups are cached by fully qualified name.
- Signature matching now compares parameter types correctly on both legacy and modern layouts.

## Unity API Wrappers (`Unity/API/`)

All wrappers resolve function pointers defensively and generally support both:

- classic object-`this` calling style
- injected `System.IntPtr`-`this` variants used by newer Unity branches

### `Application.hpp`

- Target frame rate: `GetTargetFrameRate`, `SetTargetFrameRate`
- Focus state: `GetIsFocused`
- Paths: `GetDataPath`, `GetPersistentDataPath`, `GetStreamingAssetsPath`
- Metadata: `GetProductName`, `GetIdentifier`, `GetUnityVersion`
- Quit: `Quit(int exitCode = 0)` with overload compatibility handling

### `Behaviour.hpp`

- `GetEnabled`, `SetEnabled`
- `GetIsActiveAndEnabled`

### `Camera.hpp`

- Static cameras: `Camera::GetCurrent`, `Camera::GetMain`
- Instance: `GetDepth`, `SetDepth`, `GetFieldOfView`, `SetFieldOfView`
- Projection helper: `WorldToScreen`

### `Component.hpp`

- `GetGameObject`
- `GetTransform`

### `Cursor.hpp`

- Visibility: `GetVisible`, `SetVisible`
- Lock mode: `GetLockState`, `SetLockState`
- Lock enum:
  - `Cursor::m_eLockMode::None`
  - `Cursor::m_eLockMode::Locked`
  - `Cursor::m_eLockMode::Confined`

### `Debug.hpp`

- `Log`, `LogWarning`, `LogError`
- Supports object and string paths.

### `GameObject.hpp`

- Static:
  - `CreatePrimitive`
  - `Find`
  - `FindWithTag`
- Instance:
  - `AddComponent`
  - `GetComponent`
  - `GetComponentInChildren`
  - `GetComponents`
  - `GetComponentByIndex`
  - `GetTransform`
  - `GetActive`, `SetActive`
  - `GetLayer`, `SetLayer`

### `Input.hpp`

- Axes: `GetAxis`, `GetAxisRaw`
- Mouse buttons: `GetMouseButton`, `GetMouseButtonDown`, `GetMouseButtonUp`
- Mouse position: `GetMousePosition` (injected and value fallback)
- Touch: `GetTouchCount`

### `LayerMask.hpp`

- `LayerToName`
- `NameToLayer`

### `Object.hpp`

- Object lifetime/name:
  - `CObject::Destroy`
  - `CObject::GetName`
- Object creation:
  - `Object::New(il2cppClass*)`
- Search helpers:
  - `FindObjectsOfType<T>(type, includeInactive)`
  - `FindObjectOfType<T>(...)`
- Supports legacy `FindObjectsOfType` and modern `FindObjectsByType` fallbacks.

### `PlayerPrefs.hpp`

- `GetInt`, `GetFloat`, `GetString`, `GetStringUtf8`
- `SetInt`, `SetFloat`, `SetString`
- `HasKey`, `DeleteKey`, `DeleteAll`, `Save`

### `Resources.hpp`

- `Load(path)`
- `Load(path, type)` / `Load(path, "Namespace.Type")`
- `FindObjectsOfTypeAll<T>(...)`
- `UnloadUnusedAssets()`

### `RenderSettings.hpp`

- Fog: `GetFog`, `SetFog`
- Fog color: `GetFogColor`, `SetFogColor`
- Ambient light: `GetAmbientLight`, `SetAmbientLight`
- Skybox: `GetSkybox`, `SetSkybox`
- Sun light: `GetSun`, `SetSun`

### `Rigidbody.hpp`

- `GetDetectCollisions`, `SetDetectCollisions`
- `GetVelocity`, `SetVelocity`  
  Injected and value-style fallbacks are both supported.

### `SceneManager.hpp`

- Scene info:
  - `GetSceneCount`
  - `GetActiveScene`, `SetActiveScene`
  - `GetSceneAt`
  - `GetSceneByName`, `GetSceneByPath`, `GetSceneByBuildIndex`
- Scene loading:
  - `LoadScene(name/index, mode)`
  - `LoadSceneAsync(name/index, mode)`
- Scene unloading:
  - `UnloadSceneAsync(scene/name/index)`
- Scene operations:
  - `MergeScenes`
  - `MoveGameObjectToScene`

### `Screen.hpp`

- `GetWidth`, `GetHeight`, `GetDpi`
- `GetFullScreen`, `SetFullScreen`

### `Time.hpp`

- `GetDeltaTime`, `GetUnscaledDeltaTime`
- `GetTime`, `GetUnscaledTime`
- `GetFixedDeltaTime`, `SetFixedDeltaTime`
- `GetTimeScale`, `SetTimeScale`
- `GetFrameCount`

### `Transform.hpp`

- Hierarchy:
  - `GetParent`, `GetRoot`
  - `GetChild`, `GetChildCount`
  - `FindChild`
- World/local transforms:
  - `GetPosition`, `SetPosition`
  - `GetRotation`, `SetRotation`
  - `GetLocalPosition`, `SetLocalPosition`
  - `GetLocalScale`, `SetLocalScale`

## Utility Layer (`Utils/`)

- `Utils/Hash.hpp`
  - Runtime and compile-time hashing.
  - `IL2CPP_HASH("...")` macro.
- `Utils/VTable.hpp`
  - Replace vtable function pointers safely.
  - Exact and masked opcode scanning.
- `Utils/Helper.hpp`
  - `Helper::GetMonoBehaviour()` using safe object-based search route.

## Data Structures (`Unity/Structures/`)

- Core IL2CPP objects/classes/types/methods/fields/properties.
- Arrays/lists/dictionaries:
  - `il2cppArray<T>`
  - `il2cppList<T>`
  - `il2cppDictionary<TKey, TValue>`
- String wrapper:
  - `System_String` with UTF-8 conversion helper.
- Engine math structs:
  - `Vector2/3/4`, `Quaternion`, `Color`, `Rect`, `Matrix4x4`, etc.

## Version Layout Notes

The IL2CPP type/method layout differs across Unity generations.

- New layout switch used by code paths: `UNITY_VERSION_2022_3_8F1`
- If not defined, legacy layout branches are used.

Define layout macros before including `IL2CPP_Resolver.hpp` in your project to match your target runtime.

## Quick Start Example

```cpp
#define IL2CPP_MAIN_MODULE "GameAssembly.dll" // optional override
#include "IL2CPP_Resolver.hpp"

void RuntimeEntry()
{
    if (!IL2CPP::Initialize(true, 90))
    {
        const char* reason = IL2CPP::UnityAPI::GetLastInitError();
        return;
    }

    // Unity metadata
    if (auto* v = Unity::Application::GetUnityVersion())
    {
        std::string unityVersion = v->ToString();
    }

    // Camera control
    if (Unity::CCamera* cam = Unity::Camera::GetMain())
    {
        cam->SetFieldOfView(95.0f);
    }

    // Cursor unlock helpers
    Unity::Cursor::SetVisible(true);
    Unity::Cursor::SetLockState(Unity::Cursor::m_eLockMode::None);

    // Time API
    Unity::Time::SetTimeScale(0.75f);
}
```

## Callback Example

```cpp
void OnUpdateTick()
{
    // Called every Update after hook initialization.
}

void InstallCallbacks()
{
    IL2CPP::Callback::OnUpdate::Add(reinterpret_cast<void*>(&OnUpdateTick));
    IL2CPP::Callback::Initialize();
}

void RemoveCallbacks()
{
    IL2CPP::Callback::Uninitialize();
}
```

## Class/Method Resolution Example

```cpp
void* p = IL2CPP::ResolveUnityMethodOrIcall(
    "UnityEngine.Time",
    "set_timeScale",
    1,
    { "UnityEngine.Time::set_timeScale" }
);

if (p)
{
    reinterpret_cast<void(UNITY_CALLING_CONVENTION)(float)>(p)(1.0f);
}
```

## Safety Model

- Most wrappers perform null checks before invoking pointers.
- Unity API wrappers keep separate pointers for multiple call signatures.
- Optional exports are not treated as hard failures.
- Resolver cache can be rebuilt per init attempt.
- Method-first resolution reduces dependency on unstable icall names.
- Overloaded methods can be resolved by explicit parameter type lists.
- Resolver/global state is safe to include from multiple translation units.

This gives safer behavior across mixed Unity/IL2CPP versions and modified builds, while still allowing manual resolver extension for heavily protected games.
