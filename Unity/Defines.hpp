#pragma once
// Calling Convention
#ifdef _WIN64
    #define UNITY_CALLING_CONVENTION __fastcall*
#elif _WIN32
    #define UNITY_CALLING_CONVENTION __cdecl*
#endif

// Camera
#define UNITY_CAMERA_CLASS                                          "UnityEngine.Camera"

#define UNITY_CAMERA_GETCURRENT                                     IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_current")
#define UNITY_CAMERA_GETMAIN                                        IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_main")
#define UNITY_CAMERA_GETDEPTH                                       IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_depth")
#define UNITY_CAMERA_SETDEPTH                                       IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_depth")
#define UNITY_CAMERA_GETFIELDOFVIEW                                 IL2CPP_RStr(UNITY_CAMERA_CLASS"::get_fieldOfView")
#define UNITY_CAMERA_SETFIELDOFVIEW                                 IL2CPP_RStr(UNITY_CAMERA_CLASS"::set_fieldOfView")
#define UNITY_CAMERA_WORLDTOSCREEN                                  IL2CPP_RStr(UNITY_CAMERA_CLASS"::WorldToScreenPoint_Injected")

// Component
#define UNITY_COMPONENT_CLASS										"UnityEngine.Component"

#define UNITY_COMPONENT_GETGAMEOBJECT								IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_gameObject")
#define UNITY_COMPONENT_GETTRANSFORM								IL2CPP_RStr(UNITY_COMPONENT_CLASS"::get_transform")

// GameObject
#define UNITY_GAMEOBJECT_CLASS										"UnityEngine.GameObject"

#define UNITY_GAMEOBJECT_ADDCOMPONENT 								IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::Internal_AddComponentWithType")
#define UNITY_GAMEOBJECT_CREATEPRIMITIVE 							IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::CreatePrimitive")
#define UNITY_GAMEOBJECT_FIND 										IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::Find")
#define UNITY_GAMEOBJECT_GETCOMPONENT 								IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentByName")
#define UNITY_GAMEOBJECT_GETCOMPONENTS								IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentsInternal")
#define UNITY_GAMEOBJECT_GETCOMPONENTINCHILDREN 					IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::GetComponentInChildren")
#define UNITY_GAMEOBJECT_GETACTIVE 									IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_active")
#define UNITY_GAMEOBJECT_GETLAYER 									IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_layer")
#define UNITY_GAMEOBJECT_GETTRANSFORM 								IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::get_transform")
#define UNITY_GAMEOBJECT_SETACTIVE 									IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_active")
#define UNITY_GAMEOBJECT_SETLAYER 									IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::set_layer")
#define UNITY_GAMEOBJECT_FINDGAMEOBJECTWITHTAG					    IL2CPP_RStr(UNITY_GAMEOBJECT_CLASS"::FindGameObjectsWithTag")

// LayerMask
#define UNITY_LAYERMASK_CLASS										"UnityEngine.LayerMask"

#define UNITY_LAYERMASK_LAYERTONAME									IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::LayerToName")
#define UNITY_LAYERMASK_NAMETOLAYER									IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::NameToLayer")

// MonoBehaviour
#define UNITY_MONOBEHAVIOUR_CLASS                                   "UnityEngine.MonoBehaviour"

// Object
#define UNITY_OBJECT_CLASS											"UnityEngine.Object"

#define UNITY_OBJECT_DESTROY										IL2CPP_RStr(UNITY_OBJECT_CLASS"::Destroy")
#define UNITY_OBJECT_FINDOBJECTSOFTYPE								IL2CPP_RStr(UNITY_OBJECT_CLASS"::FindObjectsOfType(System.Type,System.Boolean)")
#define UNITY_OBJECT_GETNAME										IL2CPP_RStr(UNITY_OBJECT_CLASS"::GetName")

// Rigidbody
#define UNITY_RIGIDBODY_CLASS										"UnityEngine.Rigidbody"

#define UNITY_RIGIDBODY_GETDETECTCOLLISIONS                         IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_detectCollisions")
#define UNITY_RIGIDBODY_GETVELOCITY                                 IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::get_velocity_Injected")
#define UNITY_RIGIDBODY_SETDETECTCOLLISIONS                         IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_detectCollisions")
#define UNITY_RIGIDBODY_SETVELOCITY                                 IL2CPP_RStr(UNITY_RIGIDBODY_CLASS"::set_velocity_Injected")

// Transform
#define UNITY_TRANSFORM_CLASS										"UnityEngine.Transform"

#define UNITY_TRANSFORM_GETPARENT                                   IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::GetParent")
#define UNITY_TRANSFORM_GETROOT                                     IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::GetRoot")
#define UNITY_TRANSFORM_GETCHILD                                    IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::GetChild")
#define UNITY_TRANSFORM_GETCHILDCOUNT                               IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_childCount")
#define UNITY_TRANSFORM_FINDCHILD                                   IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::FindRelativeTransformWithPath")
#define UNITY_TRANSFORM_GETPOSITION									IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_position_Injected")
#define UNITY_TRANSFORM_GETROTATION									IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_rotation_Injected")
#define UNITY_TRANSFORM_GETLOCALPOSITION							IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localPosition_Injected")
#define UNITY_TRANSFORM_GETLOCALSCALE								IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::get_localScale_Injected")
#define UNITY_TRANSFORM_SETPOSITION									IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_position_Injected")
#define UNITY_TRANSFORM_SETROTATION									IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_rotation_Injected")
#define UNITY_TRANSFORM_SETLOCALPOSITION							IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localPosition_Injected")
#define UNITY_TRANSFORM_SETLOCALSCALE								IL2CPP_RStr(UNITY_TRANSFORM_CLASS"::set_localScale_Injected")

// RenderSettings
#define UNITY_RENDERSETTINGS_CLASS                                  "UnityEngine.RenderSettings"

#define UNITY_RENDERSETTINGS_GET_FOG                                IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fog")
#define UNITY_RENDERSETTINGS_SET_FOG                                IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fog")
#define UNITY_RENDERSETTINGS_GET_FOGCOLOR                           IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_fogColor")
#define UNITY_RENDERSETTINGS_SET_FOGCOLOR                           IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_fogColor")
#define UNITY_RENDERSETTINGS_GET_AMBIENT                            IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_ambientLight")
#define UNITY_RENDERSETTINGS_SET_AMBIENT                            IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_ambientLight")
#define UNITY_RENDERSETTINGS_GET_SKYBOX                             IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_skybox")
#define UNITY_RENDERSETTINGS_SET_SKYBOX                             IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_skybox")
#define UNITY_RENDERSETTINGS_GET_SUN                                IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::get_sun")
#define UNITY_RENDERSETTINGS_SET_SUN                                IL2CPP_RStr(UNITY_RENDERSETTINGS_CLASS"::set_sun")

// Time
#define UNITY_TIME_CLASS                                            "UnityEngine.Time"

#define UNITY_TIME_GET_DELTATIME                                    IL2CPP_RStr(UNITY_TIME_CLASS"::get_deltaTime")
#define UNITY_TIME_GET_UNSCALEDDT                                   IL2CPP_RStr(UNITY_TIME_CLASS"::get_unscaledDeltaTime")
#define UNITY_TIME_GET_TIME                                         IL2CPP_RStr(UNITY_TIME_CLASS"::get_time")
#define UNITY_TIME_GET_UNSCALEDTIME                                 IL2CPP_RStr(UNITY_TIME_CLASS"::get_unscaledTime")
#define UNITY_TIME_GET_FIXEDDT                                      IL2CPP_RStr(UNITY_TIME_CLASS"::get_fixedDeltaTime")
#define UNITY_TIME_SET_FIXEDDT                                      IL2CPP_RStr(UNITY_TIME_CLASS"::set_fixedDeltaTime")
#define UNITY_TIME_GET_TIMESCALE                                    IL2CPP_RStr(UNITY_TIME_CLASS"::get_timeScale")
#define UNITY_TIME_SET_TIMESCALE                                    IL2CPP_RStr(UNITY_TIME_CLASS"::set_timeScale")
#define UNITY_TIME_GET_FRAMECOUNT                                   IL2CPP_RStr(UNITY_TIME_CLASS"::get_frameCount")

// Application
#define UNITY_APPLICATION_CLASS                                     "UnityEngine.Application"

#define UNITY_APPLICATION_GET_TARGETFRAMERATE                       IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_targetFrameRate")
#define UNITY_APPLICATION_SET_TARGETFRAMERATE                       IL2CPP_RStr(UNITY_APPLICATION_CLASS"::set_targetFrameRate")
#define UNITY_APPLICATION_GET_ISFOCUSED                             IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_isFocused")
#define UNITY_APPLICATION_GET_DATAPATH                              IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_dataPath")
#define UNITY_APPLICATION_GET_PERSISTENT                            IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_persistentDataPath")
#define UNITY_APPLICATION_GET_STREAMING                             IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_streamingAssetsPath")
#define UNITY_APPLICATION_GET_PRODUCTNAME                           IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_productName")
#define UNITY_APPLICATION_GET_IDENTIFIER                            IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_identifier")
#define UNITY_APPLICATION_GET_UNITYVERSION                          IL2CPP_RStr(UNITY_APPLICATION_CLASS"::get_unityVersion")
#define UNITY_APPLICATION_QUIT0                                     IL2CPP_RStr(UNITY_APPLICATION_CLASS"::Quit")
#define UNITY_APPLICATION_QUIT1                                     IL2CPP_RStr(UNITY_APPLICATION_CLASS"::Quit(System.Int32)")

// SceneManagement
#define UNITY_SCENEMANAGER_CLASS                                    "UnityEngine.SceneManagement.SceneManager"
#define UNITY_SCENE_CLASS                                           "UnityEngine.SceneManagement.Scene"
#define UNITY_ASYNCOP_CLASS                                         "UnityEngine.AsyncOperation"

#define UNITY_SM_SCENECOUNT                                         IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::get_sceneCount")
#define UNITY_SM_GETACTIVESCENE                                     IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetActiveScene")
#define UNITY_SM_SETACTIVESCENE                                     IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::SetActiveScene")
#define UNITY_SM_GETSCENEAT                                         IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneAt")
#define UNITY_SM_GETSCENEBYNAME                                     IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneByName")
#define UNITY_SM_GETSCENEBYPATH                                     IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneByPath")
#define UNITY_SM_GETSCENEBYBUILDINDEX                               IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::GetSceneByBuildIndex")
#define UNITY_SM_LOADSCENE_NAME                                     IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadScene(System.String)")
#define UNITY_SM_LOADSCENE_INDEX                                    IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadScene(System.Int32)")
#define UNITY_SM_LOADSCENE_NAME_MODE                                IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadScene(System.String,UnityEngine.SceneManagement.LoadSceneMode)")
#define UNITY_SM_LOADSCENE_INDEX_MODE                               IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadScene(System.Int32,UnityEngine.SceneManagement.LoadSceneMode)")
#define UNITY_SM_LOADSCENEASYNC_NAME                                IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadSceneAsync(System.String)")
#define UNITY_SM_LOADSCENEASYNC_INDEX                               IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadSceneAsync(System.Int32)")
#define UNITY_SM_LOADSCENEASYNC_NAME_MODE                           IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadSceneAsync(System.String,UnityEngine.SceneManagement.LoadSceneMode)")
#define UNITY_SM_LOADSCENEASYNC_INDEX_MODE                          IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::LoadSceneAsync(System.Int32,UnityEngine.SceneManagement.LoadSceneMode)")
#define UNITY_SM_UNLOADSCENEASYNC_SCENE                             IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::UnloadSceneAsync(UnityEngine.SceneManagement.Scene)")
#define UNITY_SM_UNLOADSCENEASYNC_NAME                              IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::UnloadSceneAsync(System.String)")
#define UNITY_SM_UNLOADSCENEASYNC_INDEX                             IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::UnloadSceneAsync(System.Int32)")
#define UNITY_SM_MERGESCENES                                        IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::MergeScenes")
#define UNITY_SM_MOVEGAMEOBJECTTOSCENE                              IL2CPP_RStr(UNITY_SCENEMANAGER_CLASS"::MoveGameObjectToScene")

// Debug
#define UNITY_DEBUG_CLASS                                           "UnityEngine.Debug"

#define UNITY_DEBUG_LOG_OBJ                                         IL2CPP_RStr(UNITY_DEBUG_CLASS"::Log(System.Object)")
#define UNITY_DEBUG_LOGWARN_OBJ                                     IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogWarning(System.Object)")
#define UNITY_DEBUG_LOGERR_OBJ                                      IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogError(System.Object)")
#define UNITY_DEBUG_LOG_STR                                         IL2CPP_RStr(UNITY_DEBUG_CLASS"::Log(System.String)")
#define UNITY_DEBUG_LOGWARN_STR                                     IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogWarning(System.String)")
#define UNITY_DEBUG_LOGERR_STR                                      IL2CPP_RStr(UNITY_DEBUG_CLASS"::LogError(System.String)")

// Input
#define UNITY_INPUT_CLASS                                           "UnityEngine.Input"

#define UNITY_INPUT_GETAXIS                                         IL2CPP_RStr(UNITY_INPUT_CLASS"::GetAxis")
#define UNITY_INPUT_GETAXISRAW                                      IL2CPP_RStr(UNITY_INPUT_CLASS"::GetAxisRaw")
#define UNITY_INPUT_GETMOUSEBUTTON                                  IL2CPP_RStr(UNITY_INPUT_CLASS"::GetMouseButton")
#define UNITY_INPUT_GETMOUSEBUTTONDOWN                              IL2CPP_RStr(UNITY_INPUT_CLASS"::GetMouseButtonDown")
#define UNITY_INPUT_GETMOUSEBUTTONUP                                IL2CPP_RStr(UNITY_INPUT_CLASS"::GetMouseButtonUp")
#define UNITY_INPUT_GETMOUSEPOSITION                                IL2CPP_RStr(UNITY_INPUT_CLASS"::get_mousePosition")
#define UNITY_INPUT_GETMOUSEPOSITION_INJECTED                       IL2CPP_RStr(UNITY_INPUT_CLASS"::get_mousePosition_Injected")
#define UNITY_INPUT_GETTOUCHCOUNT                                   IL2CPP_RStr(UNITY_INPUT_CLASS"::get_touchCount")

// Screen
#define UNITY_SCREEN_CLASS                                          "UnityEngine.Screen"

#define UNITY_SCREEN_GETWIDTH                                       IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_width")
#define UNITY_SCREEN_GETHEIGHT                                      IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_height")
#define UNITY_SCREEN_GETDPI                                         IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_dpi")
#define UNITY_SCREEN_GETFULLSCREEN                                  IL2CPP_RStr(UNITY_SCREEN_CLASS"::get_fullScreen")
#define UNITY_SCREEN_SETFULLSCREEN                                  IL2CPP_RStr(UNITY_SCREEN_CLASS"::set_fullScreen")

// Cursor
#define UNITY_CURSOR_CLASS                                          "UnityEngine.Cursor"

#define UNITY_CURSOR_GETVISIBLE                                     IL2CPP_RStr(UNITY_CURSOR_CLASS"::get_visible")
#define UNITY_CURSOR_SETVISIBLE                                     IL2CPP_RStr(UNITY_CURSOR_CLASS"::set_visible")
#define UNITY_CURSOR_GETLOCKSTATE                                   IL2CPP_RStr(UNITY_CURSOR_CLASS"::get_lockState")
#define UNITY_CURSOR_SETLOCKSTATE                                   IL2CPP_RStr(UNITY_CURSOR_CLASS"::set_lockState")





namespace Unity
{
    enum m_eTypeEnum
    {
        Type_Void = 1,
        Type_Boolean = 2,
        Type_Character = 3,
        Type_Integer = 8,
        Type_Float = 12,
        Type_String = 14,
        Type_Pointer = 15,
        Type_ValueType = 17,
        Type_Class = 18,
        Type_Variable = 19,
        Type_Array = 20,
        Type_Enum = 85,
	};

    enum m_eFieldAttribute
    {
        FieldAttribute_Compiler,
        FieldAttribute_Private,
        FieldAttribute_FamilyAndAssembly,
        FieldAttribute_Assembly,
        FieldAttribute_Family,
        FieldAttribute_FamilyOrAssembly,
        FieldAttribute_Public,
        FieldAttribute_AccessMask,
        FieldAttribute_Static = 16,
        FieldAttribute_InitOnly = 32,
        FieldAttribute_Literal = 64,
        FieldAttribute_NotSerialized = 128,
        FieldAttribute_HasRVA = 256,
        FieldAttribute_SpecialName = 512,
        FieldAttribute_RTSpecialName = 1024,
        FieldAttribute_HasMarshal = 4096,
        FieldAttribute_InvokeImpl = 8192,
        FieldAttribute_Default = 32768,
        FieldAttribute_Reserved = 38144,
    };
}
