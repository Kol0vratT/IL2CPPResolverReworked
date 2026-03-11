#pragma once

namespace IL2CPP
{
	namespace String
	{
		inline Unity::System_String* New(const char* m_String)
		{
			if (!m_String)
				m_String = "";

			if (!Functions.m_StringNew)
				return nullptr;

			return reinterpret_cast<Unity::System_String*(IL2CPP_CALLING_CONVENTION)(const char*)>(Functions.m_StringNew)(m_String);
		}

		inline Unity::System_String* New(const std::string& m_String)
		{ 
			return New(m_String.c_str());
		}

		namespace NoGC
		{
			inline Unity::System_String* New(const char* m_String)
			{
				if (!m_String)
					m_String = "";

				const int m_LengthWithNull = MultiByteToWideChar(CP_UTF8, 0, m_String, -1, nullptr, 0);
				if (m_LengthWithNull <= 0)
					return nullptr;

				const int m_Length = m_LengthWithNull - 1;
				const size_t m_AllocationSize = sizeof(Unity::System_String) + (sizeof(wchar_t) * static_cast<size_t>(m_Length));

				Unity::System_String* m_NewString = reinterpret_cast<Unity::System_String*>(::operator new(m_AllocationSize, std::nothrow));
				if (!m_NewString)
					return nullptr;

				memset(m_NewString, 0, m_AllocationSize);
				m_NewString->m_pClass = IL2CPP::Class::Find(IL2CPP_RStr("System.String"));
				m_NewString->m_pMonitor = nullptr;
				m_NewString->m_iLength = m_Length;
				MultiByteToWideChar(CP_UTF8, 0, m_String, -1, m_NewString->m_wString, m_LengthWithNull);
				return m_NewString;
			}

			inline Unity::System_String* New(const std::string& m_String)
			{
				return New(m_String.c_str());
			}
		}
	}
}
