# IL2CPP Resolver Sample DLL

Minimal DLL template for `IL2CPP_Resolver Reworked`.

The whole example lives in one file:

- `src/main.cpp`

What it does:

- creates a console;
- waits for the IL2CPP module and calls `IL2CPP::Initialize(true, 90)`;
- attaches the worker thread with `IL2CPP::ScopedThreadAttachment`;
- prints core IL2CPP smoke tests immediately;
- runs Unity API smoke tests on the Unity main thread through `Callback::OnUpdate`;
- treats optional APIs as unavailable when they are stripped from the target build;
- waits for `END` and unloads the DLL.

What to change first:

- override `IL2CPP_MAIN_MODULE` near the top of `src/main.cpp` if the target uses a different module name;
- enable `UNITY_VERSION_PRE_2022_3_8F1` there for older Unity layouts if needed;
- trim or extend the smoke-test list for the specific game you are targeting.

Build:

- open `IL2CPP_Resolver_Sample_DLL.sln`;
- build `x64`;
- the DLL will be created in `bin/Debug` or `bin/Release`.
