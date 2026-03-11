#pragma once

namespace IL2CPP
{
	namespace SystemTypeCache
	{
		inline std::unordered_map<uint32_t, Unity::il2cppObject*> m_Map;
		inline std::mutex m_MapMutex;

		inline void Clear()
		{
			std::lock_guard<std::mutex> m_Lock(m_MapMutex);
			m_Map.clear();
		}

		inline void Add(uint32_t m_Hash, Unity::il2cppObject* m_SystemType)
		{
			std::lock_guard<std::mutex> m_Lock(m_MapMutex);
			m_Map[m_Hash] = m_SystemType;
		}

		inline void Add(const char* m_Name, Unity::il2cppObject* m_SystemType)
		{
			if (!m_Name)
				return;

			Add(Utils::Hash::Get(m_Name), m_SystemType);
		}

		inline bool Contains(uint32_t m_Hash)
		{
			std::lock_guard<std::mutex> m_Lock(m_MapMutex);
			return m_Map.find(m_Hash) != m_Map.end();
		}

		inline bool Contains(const char* m_Name)
		{
			return m_Name ? Contains(Utils::Hash::Get(m_Name)) : false;
		}

		inline Unity::il2cppObject* Get(uint32_t m_Hash)
		{
			std::lock_guard<std::mutex> m_Lock(m_MapMutex);
			auto m_It = m_Map.find(m_Hash);
			return m_It == m_Map.end() ? nullptr : m_It->second;
		}

		inline Unity::il2cppObject* Get(const char* m_Name)
		{
			return m_Name ? Get(Utils::Hash::Get(m_Name)) : nullptr;
		}

		// Legacy Naming
		inline Unity::il2cppObject* Find(uint32_t m_Hash)
		{
			return Get(m_Hash);
		}

		inline Unity::il2cppObject* Find(const char* m_Name)
		{
			return Get(m_Name);
		}

		namespace Initializer
		{
			inline std::vector<const char*> m_List;
			inline std::mutex m_ListMutex;

			inline void Add(const char* m_Name)
			{
				if (!m_Name)
					return;

				std::lock_guard<std::mutex> m_Lock(m_ListMutex);
				m_List.emplace_back(m_Name);
			}

			inline void Clear()
			{
				std::lock_guard<std::mutex> m_Lock(m_ListMutex);
				m_List.clear();
			}

			inline void PreCache()
			{
				std::vector<const char*> m_LocalList;
				{
					std::lock_guard<std::mutex> m_Lock(m_ListMutex);
					m_LocalList.swap(m_List);
				}

				std::unordered_set<uint32_t> m_Seen;
				for (const char* m_Name : m_LocalList)
				{
					if (!m_Name)
						continue;

					const uint32_t m_Hash = Utils::Hash::Get(m_Name);
					if (!m_Seen.insert(m_Hash).second)
						continue;

					SystemTypeCache::Add(m_Hash, IL2CPP::Class::GetSystemType(m_Name));
				}
			}
		}
	}
}
