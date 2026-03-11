#pragma once

namespace Unity
{
	template<typename TKey,typename TValue>
	struct il2cppDictionary : il2cppObject
	{
		struct Entry
		{
			int m_iHashCode;
			int m_iNext;
			TKey m_tKey;
			TValue m_tValue;
		};
		il2cppArray<int>* m_pBuckets;
		il2cppArray<Entry>* m_pEntries;
		int m_iCount;
		int m_iVersion;
		int m_iFreeList;
		int m_iFreeCount;
		void* m_pComparer;
		void* m_pKeys;
		void* m_pValues;

		inline Entry* GetEntry()
		{
			return m_pEntries ? m_pEntries->Data() : nullptr;
		}

		inline TKey GetKeyByIndex(int iIndex)
		{
			TKey tKey = { 0 };
			
			Entry* pEntry = GetEntry();
			if (pEntry && iIndex >= 0 && iIndex < m_iCount)
				tKey = pEntry[iIndex].m_tKey;

			return tKey;
		}

		inline TValue GetValueByIndex(int iIndex)
		{
			TValue tValue = { 0 };

			Entry* pEntry = GetEntry();
			if (pEntry && iIndex >= 0 && iIndex < m_iCount)
				tValue = pEntry[iIndex].m_tValue;

			return tValue;
		}

		inline TValue GetValueByKey(TKey tKey)
		{
			TValue tValue = { 0 };

			Entry* pEntry = GetEntry();
			if (!pEntry)
				return tValue;

			for (int i = 0; i < m_iCount; ++i)
			{
				if (pEntry[i].m_tKey == tKey)
					tValue = pEntry[i].m_tValue;
			}

			return tValue;
		}
	};
}
