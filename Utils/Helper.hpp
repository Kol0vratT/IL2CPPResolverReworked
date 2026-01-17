#pragma once

namespace IL2CPP
{
	namespace Helper
	{
		Unity::CComponent* GetMonoBehaviour()
		{
			// In Unity 6 (6000.x) some GameObject icalls (notably GetComponentsInternal)
			// are frequently switched to *_Injected and/or require different argument marshalling.
			// The old approach (scan all GameObjects and call GetComponent(s)) was therefore
			// a common crash point.
			//
			// We now query MonoBehaviour instances directly via Object.FindObjectsOfType/ByType,
			// which avoids touching the brittle GameObject component enumeration path.
			Unity::il2cppArray<Unity::CComponent*>* m_MonoBehaviours = Unity::Object::FindObjectsOfType<Unity::CComponent>(UNITY_MONOBEHAVIOUR_CLASS, true);
			if (!m_MonoBehaviours || m_MonoBehaviours->m_uMaxLength == 0U)
				return nullptr;

			for (uintptr_t u = 0U; m_MonoBehaviours->m_uMaxLength > u; ++u)
			{
				Unity::CComponent* mb = m_MonoBehaviours->operator[](static_cast<unsigned int>(u));
				if (mb && mb->m_CachedPtr)
					return mb;
			}

			return nullptr;
		}
	}
}
