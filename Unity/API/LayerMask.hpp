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

			auto resolveStatic = [&](void*& outPtr,
				const char* methodName,
				std::initializer_list<const char*> primarySignature,
				std::initializer_list<const char*> secondarySignature,
				int argCount,
				std::initializer_list<const char*> icallNames)
				{
					outPtr = IL2CPP::ResolveUnityMethodOrIcall(UNITY_LAYERMASK_CLASS, methodName, primarySignature, icallNames);
					if (outPtr || secondarySignature.size() == 0U)
						return;

					outPtr = IL2CPP::ResolveUnityMethodOrIcall(UNITY_LAYERMASK_CLASS, methodName, secondarySignature, icallNames);
					if (outPtr)
						return;

					outPtr = IL2CPP::ResolveUnityMethodOrIcall(UNITY_LAYERMASK_CLASS, methodName, argCount, icallNames);
				};

			resolveStatic(m_LayerMaskFunctions.m_LayerToName,
				"LayerToName",
				{ "System.Int32" },
				{ "System.UInt32" },
				1,
				{
					UNITY_LAYERMASK_LAYERTONAME,
					IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::LayerToName(System.Int32)"),
					IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::LayerToName(System.UInt32)")
				});

			resolveStatic(m_LayerMaskFunctions.m_NameToLayer,
				"NameToLayer",
				{ "System.String" },
				{},
				1,
				{
					UNITY_LAYERMASK_NAMETOLAYER,
					IL2CPP_RStr(UNITY_LAYERMASK_CLASS"::NameToLayer(System.String)")
				});
		}

		inline System_String* LayerToName(unsigned int m_uLayer)
		{
			if (!m_LayerMaskFunctions.m_LayerToName)
				return nullptr;

			return reinterpret_cast<System_String * (UNITY_CALLING_CONVENTION)(int)>(m_LayerMaskFunctions.m_LayerToName)(static_cast<int>(m_uLayer));
		}

		inline uint32_t NameToLayer(const char* m_pName)
		{
			if (!m_LayerMaskFunctions.m_NameToLayer || !m_pName)
				return static_cast<uint32_t>(-1);

			System_String* name = IL2CPP::String::New(m_pName);
			if (!name)
				return static_cast<uint32_t>(-1);

			return reinterpret_cast<uint32_t(UNITY_CALLING_CONVENTION)(System_String*)>(m_LayerMaskFunctions.m_NameToLayer)(name);
		}
	}
}
