#pragma once

namespace Unity
{
	struct System_String : il2cppObject
	{
		int m_iLength;
		wchar_t m_wString[1];

		inline int Length() const
		{
			return m_iLength;
		}

		inline const wchar_t* Data() const
		{
			return m_wString;
		}

		inline void Clear()
		{
			if (!this) return;

			memset(m_wString, 0, static_cast<size_t>(m_iLength + 1) * sizeof(wchar_t));
			m_iLength = 0;
		}

		inline std::string ToString() const
		{
		    if (!this || m_iLength <= 0)
				return {};

			const int m_RequiredBytes = WideCharToMultiByte(CP_UTF8, 0, m_wString, m_iLength, nullptr, 0, nullptr, nullptr);
			if (m_RequiredBytes <= 0)
				return {};

			std::string sRet(static_cast<size_t>(m_RequiredBytes), '\0');
			WideCharToMultiByte(CP_UTF8, 0, m_wString, m_iLength, &sRet[0], m_RequiredBytes, nullptr, nullptr);
			return sRet;
		}
	};
}
