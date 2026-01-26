
## IL2CPP Resolver Reworked

A run-time API resolver for IL2CPP Unity.

`IL2CPP Resolver Reworked` is a C++ runtime resolver for IL2CPP-based Unity games.
It provides a high-level and low-level API for resolving IL2CPP exports, accessing managed classes, invoking methods, working with fields, strings, and interacting with Unity engine objects.

---

# DOCUMENTATION

---

## Quick Example

```cpp
#include "IL2CPP_Resolver.hpp"

void SomeFunction()
{
    IL2CPP::Initialize(); // This needs to be called once!

    Unity::CGameObject* m_Local = Unity::GameObject::Find("LocalPlayer");
    Unity::CComponent* m_LocalData = m_Local->GetComponent("PlayerData");
    m_LocalData->SetMemberValue<bool>("CanFly", true);
}
```

---

## Registering OnUpdate Callback

```cpp
void OurUpdateFunction()
{
    // Your special code...
}

void OnLoad()
{
    IL2CPP::Initialize();

    IL2CPP::Callback::Initialize();
    IL2CPP::Callback::OnUpdate::Add(OurUpdateFunction);
}
```

---

## Initialization

### IL2CPP::Initialize

```cpp
bool IL2CPP::Initialize();
```

Initializes the IL2CPP resolver.
This function **must be called once** before using any other API.

What this function does:

* Resolves `GameAssembly.dll`
* Resolves all required IL2CPP exports
* Initializes IL2CPP domain
* Attaches current thread
* Caches base system types

Example:

```cpp
if (!IL2CPP::Initialize())
{
    // Initialization failed
}
```

---

## Domain API

### IL2CPP::Domain::Get

```cpp
Il2CppDomain* IL2CPP::Domain::Get();
```

Returns the current IL2CPP domain.

---

### IL2CPP::Domain::GetAssemblies

```cpp
std::vector<Il2CppAssembly*> IL2CPP::Domain::GetAssemblies();
```

Returns a list of all loaded assemblies.

Example:

```cpp
auto assemblies = IL2CPP::Domain::GetAssemblies();
for (auto assembly : assemblies)
{
    printf("Assembly: %s\n", assembly->image->name);
}
```

---

## Class API

### IL2CPP::Class::Find

```cpp
Il2CppClass* IL2CPP::Class::Find(
    const char* namespaze,
    const char* className,
    const char* assemblyName = nullptr
);
```

Finds an IL2CPP class by namespace and name.

Example:

```cpp
Il2CppClass* playerClass =
    IL2CPP::Class::Find("Game", "Player");
```

---

### IL2CPP::Class::GetMethod

```cpp
MethodInfo* IL2CPP::Class::GetMethod(
    Il2CppClass* klass,
    const char* methodName,
    int argsCount = -1
);
```

Returns a method from the specified class.

Example:

```cpp
MethodInfo* jumpMethod =
    IL2CPP::Class::GetMethod(playerClass, "Jump", 0);
```

---

### IL2CPP::Class::GetField

```cpp
FieldInfo* IL2CPP::Class::GetField(
    Il2CppClass* klass,
    const char* fieldName
);
```

Returns a field from the specified class.

---

## Method Invocation

### IL2CPP::ResolveCall::Invoke

```cpp
template<typename Ret, typename... Args>
Ret IL2CPP::ResolveCall::Invoke(
    MethodInfo* method,
    void* instance,
    Args... args
);
```

Invokes a managed IL2CPP method.

Example:

```cpp
bool result = IL2CPP::ResolveCall::Invoke<bool>(
    someMethod,
    instance,
    123
);
```

---

## String API

### IL2CPP::String::New

```cpp
Il2CppString* IL2CPP::String::New(const char* value);
```

Creates a managed `System.String`.

Example:

```cpp
Il2CppString* str = IL2CPP::String::New("Hello World");
```

---

### IL2CPP::String::ToChars

```cpp
std::string IL2CPP::String::ToChars(Il2CppString* value);
```

Converts `System.String` to `std::string`.

---

## Callback System

### IL2CPP::Callback::Initialize

```cpp
void IL2CPP::Callback::Initialize();
```

Initializes the callback system.
Must be called after `IL2CPP::Initialize()`.

---

### IL2CPP::Callback::OnUpdate::Add

```cpp
void IL2CPP::Callback::OnUpdate::Add(void(*callback)());
```

Registers a function that will be executed every Unity `Update()`.

Example:

```cpp
void UpdateLogic()
{
    // Code executed every frame
}

void Init()
{
    IL2CPP::Initialize();
    IL2CPP::Callback::Initialize();
    IL2CPP::Callback::OnUpdate::Add(UpdateLogic);
}
```

---

## Unity API

All Unity wrappers are located in the `Unity` namespace.

---

## Unity::GameObject

### GameObject::Find

```cpp
static CGameObject* Unity::GameObject::Find(const char* name);
```

Finds a `GameObject` by name.

Example:

```cpp
Unity::CGameObject* obj =
    Unity::GameObject::Find("Player");
```

---

### GameObject::GetComponent

```cpp
Unity::CComponent* GetComponent(const char* componentName);
```

Returns a component from the game object.

Example:

```cpp
auto rigidbody = obj->GetComponent("Rigidbody");
```

---

## Unity::Component

### SetMemberValue

```cpp
template<typename T>
void SetMemberValue(const char* fieldName, T value);
```

Sets the value of a managed field.

Example:

```cpp
component->SetMemberValue<float>("Speed", 10.0f);
```

---

## Unity::Transform

Supported operations:

* GetPosition
* SetPosition
* GetRotation
* SetRotation
* GetForward

Example:

```cpp
auto transform = obj->GetTransform();
transform->SetPosition({ 0.f, 5.f, 0.f });
```

---

## Unity::Time

```cpp
float Unity::Time::GetDeltaTime();
float Unity::Time::GetTime();
```

---

## Unity::Camera

```cpp
static CCamera* Unity::Camera::GetMain();
```

---

## Unity Version Compatibility

By default, the resolver targets **Unity 2022.3.8f1 and newer**.

To support older Unity versions, define:

```cpp
#define UNITY_VERSION_PRE_2022_3_8F1
#include "IL2CPP_Resolver.hpp"
```

---

## Configuration

### Custom GameAssembly Name

```cpp
#define IL2CPP_MAIN_MODULE "CustomGameAssembly.dll"
```

---

### String Obfuscation Support

```cpp
#define IL2CPP_RStr(x) xorstr_(x)
```

---

## Summary

`IL2CPP Resolver Reworked` provides:

* Runtime IL2CPP resolving
* Clean C++ API
* Unity engine wrappers
* Safe managed method invocation
* Minimal overhead
* No hardcoded offsets
