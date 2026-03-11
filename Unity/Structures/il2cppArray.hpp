#pragma once

namespace Unity
{
	template<typename T>
	struct il2cppArray : il2cppObject
	{
		il2cppArrayBounds* m_pBounds = nullptr;
		uintptr_t m_uMaxLength = 0;
		T* m_pValues = nullptr;

		inline uintptr_t GetData() const
		{
			return reinterpret_cast<uintptr_t>(&m_pValues);
		}

		inline T* Data()
		{
			return reinterpret_cast<T*>(&m_pValues);
		}

		inline const T* Data() const
		{
			return reinterpret_cast<const T*>(&m_pValues);
		}

		inline uintptr_t Size() const
		{
			return m_uMaxLength;
		}

		inline bool Empty() const
		{
			return m_uMaxLength == 0;
		}

		inline T& operator[](unsigned int m_uIndex)
		{
			return Data()[m_uIndex];
		}

		inline const T& operator[](unsigned int m_uIndex) const
		{
			return Data()[m_uIndex];
		}

		inline T& At(unsigned int m_uIndex)
		{
			return operator[](m_uIndex);
		}

		inline const T& At(unsigned int m_uIndex) const
		{
			return operator[](m_uIndex);
		}

		inline void Insert(T* m_pArray, uintptr_t m_uSize, uintptr_t m_uIndex = 0)
		{
			if ((m_uSize + m_uIndex) >= m_uMaxLength)
			{
				if (m_uIndex >= m_uMaxLength)
					return;

				m_uSize = m_uMaxLength - m_uIndex;
			}

			for (uintptr_t u = 0; m_uSize > u; ++u)
				operator[](u + m_uIndex) = m_pArray[u];
		}

		inline void Fill(T m_tValue)
		{
			for (uintptr_t u = 0; m_uMaxLength > u; ++u)
				operator[](u) = m_tValue;
		}

		inline void RemoveAt(unsigned int m_uIndex)
		{
			if (m_uIndex >= m_uMaxLength)
				return;

			for (uintptr_t u = m_uIndex; (u + 1U) < m_uMaxLength; ++u)
				operator[](static_cast<unsigned int>(u)) = operator[](static_cast<unsigned int>(u + 1U));

			--m_uMaxLength;
		}

		inline void RemoveRange(unsigned int m_uIndex, unsigned int m_uCount)
		{
			if (m_uCount == 0)
				m_uCount = 1;

			unsigned int m_uTotal = m_uIndex + m_uCount;
			if (m_uTotal >= m_uMaxLength)
				return;

			for (uintptr_t u = m_uIndex; (u + m_uCount) < m_uMaxLength; ++u)
				operator[](static_cast<unsigned int>(u)) = operator[](static_cast<unsigned int>(u + m_uCount));

			m_uMaxLength -= m_uCount;
		}

		inline void RemoveAll()
		{
			if (m_uMaxLength > 0)
			{
				memset(GetData(), 0, sizeof(T) * m_uMaxLength);
				m_uMaxLength = 0;
			}
		}
	};

	// Defined here because its basically same shit
	template<typename T>
	struct il2cppList : il2cppObject
	{
		il2cppArray<T>* m_pListArray;

		inline il2cppArray<T>* ToArray() { return m_pListArray; }
	};
}
