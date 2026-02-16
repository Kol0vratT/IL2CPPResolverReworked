#pragma once

namespace Unity
{
	struct LayerMaskFunctions_t
	{
		void* m_LayerToName = nullptr;
		void* m_NameToLayer = nullptr;
	};
	inline LayerMaskFunctions_t m_LayerMaskFunctions;

	namespace LayerMask
	{
		inline void Initialize()
		{
			IL2CPP::SystemTypeCache::Initializer::Add(UNITY_LAYERMASK_CLASS);

			m_LayerMaskFunctions.m_LayerToName = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_LAYERMASK_CLASS, "LayerToName", 1,
				{ UNITY_LAYERMASK_LAYERTONAME, IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::LayerToName_Injected") });

			m_LayerMaskFunctions.m_NameToLayer = IL2CPP::ResolveUnityMethodOrIcall(
				UNITY_LAYERMASK_CLASS, "NameToLayer", 1,
				{ UNITY_LAYERMASK_NAMETOLAYER, IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::NameToLayer_Injected") });
		}

		inline System_String* LayerToName(unsigned int m_uLayer)
		{
			if (!m_LayerMaskFunctions.m_LayerToName)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)(unsigned int)>(m_LayerMaskFunctions.m_LayerToName)(m_uLayer);
		}

		inline uint32_t NameToLayer(const char* m_pName)
		{
			if (!m_LayerMaskFunctions.m_NameToLayer || !m_pName)
				return static_cast<uint32_t>(-1);

			System_String* name = IL2CPP::String::New(m_pName);
			if (!name)
				return static_cast<uint32_t>(-1);

			return reinterpret_cast<uint32_t(UNITY_CALLING_CONVENTION)(void*)>(m_LayerMaskFunctions.m_NameToLayer)(name);
		}
	}
}
