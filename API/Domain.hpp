#pragma once

namespace IL2CPP
{
	namespace Domain
	{
		void* Get()
		{
			if (!Functions.m_DomainGet)
				return nullptr;

			return reinterpret_cast<void*(IL2CPP_CALLING_CONVENTION)()>(Functions.m_DomainGet)();
		}

		Unity::il2cppAssembly** GetAssemblies(size_t* m_Size)
		{
			if (!Functions.m_DomainGetAssemblies)
				return nullptr;

			void* m_Domain = Get();
			if (!m_Domain)
				return nullptr;

			return reinterpret_cast<Unity::il2cppAssembly**(IL2CPP_CALLING_CONVENTION)(void*, size_t*)>(Functions.m_DomainGetAssemblies)(m_Domain, m_Size);
		}
	}
}
