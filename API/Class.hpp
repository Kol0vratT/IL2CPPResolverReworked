#pragma once

namespace IL2CPP
{
	namespace Class
	{
		namespace Cache
		{
			inline std::unordered_map<std::string, Unity::il2cppClass*> m_ClassByName;
			inline std::mutex m_ClassByNameMutex;
		}

		inline void ClearCache()
		{
			std::lock_guard<std::mutex> m_Lock(Cache::m_ClassByNameMutex);
			Cache::m_ClassByName.clear();
		}

		inline Unity::il2cppFieldInfo* GetFields(Unity::il2cppClass* m_pClass, void** m_pIterator)
		{
			if (!Functions.m_ClassGetFields || !m_pClass)
				return nullptr;

			return reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, void**)>(Functions.m_ClassGetFields)(m_pClass, m_pIterator);
		}

		inline void FetchFields(Unity::il2cppClass* m_pClass, std::vector<Unity::il2cppFieldInfo*>* m_pVector, void* m_pFieldIterator = nullptr)
		{
			if (!m_pVector)
				return;

			m_pVector->clear();

			while (true)
			{
				Unity::il2cppFieldInfo* m_pField = GetFields(m_pClass, &m_pFieldIterator);
				if (!m_pField)
					break;

				m_pVector->emplace_back(m_pField);
			}
		}

		inline Unity::il2cppMethodInfo* GetMethods(Unity::il2cppClass* m_pClass, void** m_pIterator)
		{
			if (!Functions.m_ClassGetMethods || !m_pClass)
				return nullptr;

			return reinterpret_cast<Unity::il2cppMethodInfo * (IL2CPP_CALLING_CONVENTION)(void*, void**)>(Functions.m_ClassGetMethods)(m_pClass, m_pIterator);
		}

		inline void FetchMethods(Unity::il2cppClass* m_pClass, std::vector<Unity::il2cppMethodInfo*>* m_pVector, void* m_pMethodIterator = nullptr)
		{
			if (!m_pVector)
				return;

			m_pVector->clear();

			while (true)
			{
				Unity::il2cppMethodInfo* m_pMethod = GetMethods(m_pClass, &m_pMethodIterator);
				if (!m_pMethod)
					break;

				m_pVector->emplace_back(m_pMethod);
			}
		}

		inline Unity::il2cppType* GetType(Unity::il2cppClass* m_pClass)
		{
			if (!Functions.m_ClassGetType || !m_pClass)
				return nullptr;

			return reinterpret_cast<Unity::il2cppType * (IL2CPP_CALLING_CONVENTION)(void*)>(Functions.m_ClassGetType)(m_pClass);
		}

		inline Unity::il2cppObject* GetSystemType(Unity::il2cppClass* m_pClass)
		{
			if (!Functions.m_TypeGetObject || !m_pClass)
				return nullptr;

			return reinterpret_cast<Unity::il2cppObject * (IL2CPP_CALLING_CONVENTION)(void*)>(Functions.m_TypeGetObject)(GetType(m_pClass));
		}

		inline Unity::il2cppClass* GetFromName(Unity::il2cppImage* m_pImage, const char* m_pNamespace, const char* m_pName)
		{
			if (!Functions.m_ClassFromName || !m_pImage || !m_pName)
				return nullptr;

			return reinterpret_cast<Unity::il2cppClass * (IL2CPP_CALLING_CONVENTION)(void*, const char*, const char*)>(Functions.m_ClassFromName)(m_pImage, m_pNamespace ? m_pNamespace : "", m_pName);
		}

		inline Unity::il2cppClass* Find(const char* m_pName)
		{
			if (!m_pName || !m_pName[0])
				return nullptr;

			{
				std::lock_guard<std::mutex> m_Lock(Cache::m_ClassByNameMutex);
				auto m_It = Cache::m_ClassByName.find(m_pName);
				if (m_It != Cache::m_ClassByName.end())
					return m_It->second;
			}

			size_t m_sAssembliesCount = 0U;
			Unity::il2cppAssembly** m_pAssemblies = Domain::GetAssemblies(&m_sAssembliesCount);
			if (!m_pAssemblies || m_sAssembliesCount == 0U)
				return nullptr;

			std::string m_NameSpace;
			const char* m_pClassName = m_pName;

			if (const char* m_pNameSpaceEnd = strrchr(m_pName, '.'))
			{
				m_NameSpace.assign(m_pName, static_cast<size_t>(m_pNameSpaceEnd - m_pName));
				m_pClassName = m_pNameSpaceEnd + 1;
			}

			Unity::il2cppClass* m_pClassReturn = nullptr;
			for (size_t i = 0U; i < m_sAssembliesCount; ++i)
			{
				Unity::il2cppAssembly* m_pAssembly = m_pAssemblies[i];
				if (!m_pAssembly || !m_pAssembly->m_pImage)
					continue;

				m_pClassReturn = GetFromName(m_pAssembly->m_pImage, m_NameSpace.c_str(), m_pClassName);
				if (m_pClassReturn)
					break;
			}

			std::lock_guard<std::mutex> m_Lock(Cache::m_ClassByNameMutex);
			Cache::m_ClassByName.emplace(m_pName, m_pClassReturn);
			return m_pClassReturn;
		}

		inline Unity::il2cppObject* GetSystemType(const char* m_pClassName)
		{
			Unity::il2cppClass* m_pClass = Find(m_pClassName);
			if (!m_pClass)
				return nullptr;

			return GetSystemType(m_pClass);
		}

		inline void FetchClasses(std::vector<Unity::il2cppClass*>* m_pVector, const char* m_pModuleName, const char* m_pNamespace)
		{
			if (!m_pVector)
				return;

			m_pVector->clear();

			if (!Functions.m_ImageGetClassCount || !Functions.m_ImageGetClass)
				return;

			size_t m_sAssembliesCount = 0U;
			Unity::il2cppAssembly** m_pAssemblies = Domain::GetAssemblies(&m_sAssembliesCount);
			if (!m_pAssemblies || m_sAssembliesCount == 0U)
				return;

			for (size_t i = 0U; i < m_sAssembliesCount; ++i)
			{
				Unity::il2cppAssembly* m_pAssembly = m_pAssemblies[i];
				if (!m_pAssembly || !m_pAssembly->m_pImage)
					continue;

				if (m_pModuleName && m_pModuleName[0] != '\0' && strcmp(m_pAssembly->m_pImage->m_pNameNoExt, m_pModuleName) != 0)
					continue;

				size_t m_sClassesCount = reinterpret_cast<size_t(IL2CPP_CALLING_CONVENTION)(void*)>(Functions.m_ImageGetClassCount)(m_pAssembly->m_pImage);
				for (size_t c = 0U; c < m_sClassesCount; ++c)
				{
					Unity::il2cppClass* m_pClass = reinterpret_cast<Unity::il2cppClass * (IL2CPP_CALLING_CONVENTION)(void*, size_t)>(Functions.m_ImageGetClass)(m_pAssembly->m_pImage, c);
					if (!m_pClass)
						continue;

					if (m_pNamespace)
					{
						if (m_pNamespace[0] == '\0')
						{
							if (m_pClass->m_pNamespace && m_pClass->m_pNamespace[0] != '\0')
								continue;
						}
						else if (!m_pClass->m_pNamespace || strcmp(m_pClass->m_pNamespace, m_pNamespace) != 0)
						{
							continue;
						}
					}

					m_pVector->emplace_back(m_pClass);
				}
			}
		}

		namespace Utils
		{
			inline std::string GetClassFullName(Unity::il2cppClass* m_pClass)
			{
				if (!m_pClass || !m_pClass->m_pName)
					return {};

				if (m_pClass->m_pNamespace && m_pClass->m_pNamespace[0] != '\0')
					return std::string(m_pClass->m_pNamespace) + "." + m_pClass->m_pName;

				return std::string(m_pClass->m_pName);
			}

			inline std::string NormalizeTypeName(const char* m_pTypeName)
			{
				if (!m_pTypeName)
					return {};

				std::string m_Name(m_pTypeName);
				while (!m_Name.empty() && (m_Name.back() == '&' || m_Name.back() == '*'))
					m_Name.pop_back();

				return m_Name;
			}

			inline int GetFieldOffset(Unity::il2cppClass* m_pClass, const char* m_pName)
			{
				if (!m_pClass || !m_pName)
					return -1;

				void* m_pFieldIterator = nullptr;
				while (true)
				{
					Unity::il2cppFieldInfo* m_pField = GetFields(m_pClass, &m_pFieldIterator);
					if (!m_pField)
						break;

					if (m_pField->m_pName && strcmp(m_pField->m_pName, m_pName) == 0)
						return m_pField->m_iOffset;
				}

				return -1;
			}

			inline int GetFieldOffset(const char* m_pClassName, const char* m_pName)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (m_pClass)
					return GetFieldOffset(m_pClass, m_pName);

				return -1;
			}

			inline void SetStaticField(Unity::il2cppClass* m_pClass, const char* m_pMemberName, void* m_pValue)
			{
				if (!Functions.m_ClassGetFieldFromName || !Functions.m_FieldStaticSetValue || !m_pClass || !m_pMemberName)
					return;

				Unity::il2cppFieldInfo* m_pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_pClass, m_pMemberName);
				if (m_pField)
					reinterpret_cast<void (IL2CPP_CALLING_CONVENTION)(Unity::il2cppFieldInfo*, void*)>(Functions.m_FieldStaticSetValue)(m_pField, m_pValue);
			}

			inline void SetStaticField(const char* m_pClassName, const char* m_pMemberName, void* m_pValue)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (m_pClass)
					SetStaticField(m_pClass, m_pMemberName, m_pValue);
			}

			inline void* GetStaticField(Unity::il2cppClass* m_pClass, const char* m_pMemberName)
			{
				if (!Functions.m_ClassGetFieldFromName || !Functions.m_FieldStaticGetValue || !m_pClass || !m_pMemberName)
					return nullptr;

				Unity::il2cppFieldInfo* m_pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_pClass, m_pMemberName);
				void* m_pValue = nullptr;
				if (m_pField)
					reinterpret_cast<void (IL2CPP_CALLING_CONVENTION)(Unity::il2cppFieldInfo*, void*)>(Functions.m_FieldStaticGetValue)(m_pField, &m_pValue);

				return m_pValue;
			}

			inline void* GetStaticField(const char* m_pClassName, const char* m_pMemberName)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (m_pClass)
					return GetStaticField(m_pClass, m_pMemberName);

				return nullptr;
			}

			inline Unity::il2cppMethodInfo* GetMethodInfo(Unity::il2cppClass* m_pClass, const char* m_pMethodName, int m_iArgs = -1)
			{
				if (!Functions.m_ClassGetMethodFromName || !m_pClass || !m_pMethodName)
					return nullptr;

				return reinterpret_cast<Unity::il2cppMethodInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*, int)>(Functions.m_ClassGetMethodFromName)(m_pClass, m_pMethodName, m_iArgs);
			}

			inline Unity::il2cppMethodInfo* GetMethodInfo(const char* m_pClassName, const char* m_pMethodName, int m_iArgs = -1)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (!m_pClass)
					return nullptr;

				return GetMethodInfo(m_pClass, m_pMethodName, m_iArgs);
			}

			inline Unity::il2cppClass* ClassFromType(Unity::il2cppType* m_pType)
			{
				if (!Functions.m_ClassFromIl2cppType || !m_pType)
					return nullptr;

				return reinterpret_cast<Unity::il2cppClass * (IL2CPP_CALLING_CONVENTION)(void*)>(Functions.m_ClassFromIl2cppType)(m_pType);
			}

			inline bool TypeNameMatches(Unity::il2cppType* m_pType, const char* m_pExpectedName)
			{
				if (!m_pType || !m_pExpectedName)
					return false;

				Unity::il2cppClass* m_pTypeClass = ClassFromType(m_pType);
				if (!m_pTypeClass || !m_pTypeClass->m_pName)
					return false;

				const std::string m_ExpectedName = NormalizeTypeName(m_pExpectedName);
				const std::string m_FullName = NormalizeTypeName(GetClassFullName(m_pTypeClass).c_str());
				return m_FullName == m_ExpectedName || strcmp(m_pTypeClass->m_pName, m_ExpectedName.c_str()) == 0;
			}

			inline void* GetMethodPointer(Unity::il2cppClass* m_pClass, const char* m_pMethodName, int m_iArgs = -1)
			{
				Unity::il2cppMethodInfo* m_pMethod = GetMethodInfo(m_pClass, m_pMethodName, m_iArgs);
				return m_pMethod ? m_pMethod->m_pMethodPointer : nullptr;
			}

			inline void* GetMethodPointer(const char* m_pClassName, const char* m_pMethodName, int m_iArgs = -1)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (m_pClass)
					return GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);

				return nullptr;
			}

			inline const char* MethodGetParamName(Unity::il2cppMethodInfo* m_pMethodInfo, uint32_t index)
			{
				if (!Functions.m_MethodGetParamName || !m_pMethodInfo || index >= m_pMethodInfo->m_uArgsCount)
					return nullptr;

				return reinterpret_cast<const char * (IL2CPP_CALLING_CONVENTION)(void*, uint32_t)>(Functions.m_MethodGetParamName)(m_pMethodInfo, index);
			}

			inline Unity::il2cppType* GetMethodParamType(Unity::il2cppMethodInfo* m_pMethodInfo, uint32_t index)
			{
				if (!Functions.m_MethodGetParam || !m_pMethodInfo || index >= m_pMethodInfo->m_uArgsCount)
					return nullptr;

				return reinterpret_cast<Unity::il2cppType * (IL2CPP_CALLING_CONVENTION)(void*, uint32_t)>(Functions.m_MethodGetParam)(m_pMethodInfo, index);
			}

			inline Unity::il2cppMethodInfo* GetMethodInfo(Unity::il2cppClass* m_pClass, const char* m_pMethodName, std::initializer_list<const char*> m_vNames)
			{
				if (!m_pClass || !m_pMethodName)
					return nullptr;

				const int m_iNamesCount = static_cast<int>(m_vNames.size());
				const char** m_pNames = const_cast<const char**>(m_vNames.begin());

				void* m_pMethodIterator = nullptr;
				while (true)
				{
					Unity::il2cppMethodInfo* m_pMethod = GetMethods(m_pClass, &m_pMethodIterator);
					if (!m_pMethod)
						break;

					if (!m_pMethod->m_pName || strcmp(m_pMethod->m_pName, m_pMethodName) != 0)
						continue;

					if (m_pMethod->m_uArgsCount != m_iNamesCount)
						continue;

					bool m_Matched = true;
#ifdef UNITY_VERSION_2022_3_8F1
					for (int i = 0; i < m_iNamesCount; ++i)
					{
						if (!TypeNameMatches(m_pMethod->m_pParameters[i], m_pNames[i]))
						{
							m_Matched = false;
							break;
						}
					}
#else
					for (int i = 0; i < m_iNamesCount; ++i)
					{
						if (!TypeNameMatches(m_pMethod->m_pParameters[i].m_pParameterType, m_pNames[i]))
						{
							m_Matched = false;
							break;
						}
					}
#endif
					if (m_Matched)
						return m_pMethod;
				}

				return nullptr;
			}

			inline Unity::il2cppMethodInfo* GetMethodInfo(const char* m_pClassName, const char* m_pMethodName, std::initializer_list<const char*> m_vNames)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (!m_pClass)
					return nullptr;

				return GetMethodInfo(m_pClass, m_pMethodName, m_vNames);
			}

			inline void* GetMethodPointer(Unity::il2cppClass* m_pClass, const char* m_pMethodName, std::initializer_list<const char*> m_vNames)
			{
				Unity::il2cppMethodInfo* m_pMethod = GetMethodInfo(m_pClass, m_pMethodName, m_vNames);
				return m_pMethod ? m_pMethod->m_pMethodPointer : nullptr;
			}

			inline void* GetMethodPointer(const char* m_pClassName, const char* m_pMethodName, std::initializer_list<const char*> m_vNames)
			{
				Unity::il2cppClass* m_pClass = Find(m_pClassName);
				if (!m_pClass)
					return nullptr;

				return GetMethodPointer(m_pClass, m_pMethodName, m_vNames);
			}

			inline Unity::il2cppClass* FilterClass(std::vector<Unity::il2cppClass*>* m_pClasses, std::initializer_list<const char*> m_vNames, int m_iFoundCount = -1)
			{
				if (!m_pClasses)
					return nullptr;

				const int m_iNamesCount = static_cast<int>(m_vNames.size());
				const char** m_pNames = const_cast<const char**>(m_vNames.begin());

				if (m_iFoundCount <= 0 || m_iFoundCount > m_iNamesCount)
					m_iFoundCount = m_iNamesCount;

				Unity::il2cppClass* m_pReturn = nullptr;
				for (size_t c = 0; c < m_pClasses->size(); ++c)
				{
					int m_iFoundCountNow = 0;

					Unity::il2cppClass* m_pClass = m_pClasses->operator[](c);
					if (!m_pClass)
						continue;

					for (int i = 0; i < m_iNamesCount; ++i)
					{
						const char* m_pNameToFind = m_pNames[i];
						if (!m_pNameToFind || !m_pNameToFind[0])
							continue;

						bool m_bFoundInClass = false;
						if (m_pNameToFind[0] == '~')
							m_bFoundInClass = GetFieldOffset(m_pClass, &m_pNameToFind[1]) >= 0;
						else if (m_pNameToFind[0] == '-')
							m_bFoundInClass = GetMethodPointer(m_pClass, &m_pNameToFind[1]) != nullptr;
						else
						{
							m_bFoundInClass = GetFieldOffset(m_pClass, m_pNameToFind) >= 0;
							if (!m_bFoundInClass)
								m_bFoundInClass = GetMethodPointer(m_pClass, m_pNameToFind) != nullptr;
						}

						if (m_bFoundInClass)
							++m_iFoundCountNow;
					}

					if (m_iFoundCount == m_iFoundCountNow)
					{
						m_pReturn = m_pClass;
						break;
					}
				}

				return m_pReturn;
			}

			inline void* FilterClassToMethodPointer(std::vector<Unity::il2cppClass*>* m_pClasses, const char* m_pMethodName, int m_iArgs = -1)
			{
				if (!m_pClasses)
					return nullptr;

				void* m_pMethodPointer = nullptr;
				for (size_t c = 0; c < m_pClasses->size(); ++c)
				{
					Unity::il2cppClass* m_pClass = m_pClasses->operator[](c);
					if (!m_pClass)
						continue;

					m_pMethodPointer = GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);
					if (m_pMethodPointer)
						break;
				}

				return m_pMethodPointer;
			}
		}
	}

	enum class m_eClassPropType : int
	{
		Unknown = 0,
		Field,
		Property,
		Method,
	};

	class CClass
	{
	public:
		Unity::il2cppObject m_Object = { 0 };
		void* m_CachedPtr = nullptr;

		inline bool IsValid() const
		{
			return m_Object.m_pClass != nullptr;
		}

		inline Unity::il2cppObject* GetManagedObject()
		{
			return reinterpret_cast<Unity::il2cppObject*>(&m_Object);
		}

		inline const Unity::il2cppObject* GetManagedObject() const
		{
			return reinterpret_cast<const Unity::il2cppObject*>(&m_Object);
		}

		inline void* GetManagedObjectPointer()
		{
			return static_cast<void*>(&m_Object);
		}

		inline const void* GetManagedObjectPointer() const
		{
			return static_cast<const void*>(&m_Object);
		}

		inline void* GetNativeObjectPointer() const
		{
			return m_CachedPtr;
		}

		inline Unity::il2cppFieldInfo* GetFields(void** m_pIterator)
		{
			return Class::GetFields(m_Object.m_pClass, m_pIterator);
		}

		inline void FetchFields(std::vector<Unity::il2cppFieldInfo*>* m_pVector, void* m_pFieldIterator = nullptr)
		{
			Class::FetchFields(m_Object.m_pClass, m_pVector, m_pFieldIterator);
		}

		inline Unity::il2cppMethodInfo* GetMethods(void** m_pIterator)
		{
			return Class::GetMethods(m_Object.m_pClass, m_pIterator);
		}

		inline void FetchMethods(std::vector<Unity::il2cppMethodInfo*>* m_pVector, void* m_pMethodIterator = nullptr)
		{
			Class::FetchMethods(m_Object.m_pClass, m_pVector, m_pMethodIterator);
		}

		inline void* GetMethodPointer(const char* m_pMethodName, int m_iArgs = -1)
		{
			return Class::Utils::GetMethodPointer(m_Object.m_pClass, m_pMethodName, m_iArgs);
		}

		inline m_eClassPropType GetPropType(const char* m_pPropType)
		{
			if (!m_pPropType || !m_Object.m_pClass)
				return m_eClassPropType::Unknown;

			if (Functions.m_ClassGetFieldFromName)
			{
				Unity::il2cppFieldInfo* pField = reinterpret_cast<Unity::il2cppFieldInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_Object.m_pClass, m_pPropType);
				if (pField)
					return m_eClassPropType::Field;
			}

			if (Functions.m_ClassGetPropertyFromName)
			{
				Unity::il2cppPropertyInfo* pProperty = reinterpret_cast<Unity::il2cppPropertyInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetPropertyFromName)(m_Object.m_pClass, m_pPropType);
				if (pProperty)
					return m_eClassPropType::Property;
			}

			if (Functions.m_ClassGetMethodFromName)
			{
				Unity::il2cppMethodInfo* pMethod = reinterpret_cast<Unity::il2cppMethodInfo*(IL2CPP_CALLING_CONVENTION)(void*, const char*, int)>(Functions.m_ClassGetMethodFromName)(m_Object.m_pClass, m_pPropType, -1);
				if (pMethod)
					return m_eClassPropType::Method;
			}

			return m_eClassPropType::Unknown;
		}

		template<typename TReturn, typename... TArgs>
		TReturn CallMethod(void* m_pMethod, TArgs... tArgs)
		{
			return reinterpret_cast<TReturn(UNITY_CALLING_CONVENTION)(void*, TArgs...)>(m_pMethod)(GetManagedObjectPointer(), tArgs...);
		}

		template<typename TReturn, typename... TArgs>
		TReturn CallMethod(const char* m_pMethodName, TArgs... tArgs)
		{
			return CallMethod<TReturn>(GetMethodPointer(m_pMethodName), tArgs...);
		}

		template<typename TReturn, typename... TArgs>
		TReturn CallMethodSafe(void* m_pMethod, TArgs... tArgs)
		{
			if (!m_pMethod)
			{
#ifdef _DEBUG
				__debugbreak();
#endif
				TReturn m_tDefault = {};
				return m_tDefault;
			}

			return CallMethod<TReturn>(m_pMethod, tArgs...);
		}

		template<typename TReturn, typename... TArgs>
		TReturn CallMethodSafe(const char* m_pMethodName, TArgs... tArgs)
		{
			return CallMethodSafe<TReturn>(GetMethodPointer(m_pMethodName), tArgs...);
		}

		template<typename T>
		T GetPropertyValue(const char* m_pPropertyName)
		{
			if (!Functions.m_ClassGetPropertyFromName || !m_Object.m_pClass || !m_pPropertyName)
			{
				T tDefault = {};
				return tDefault;
			}

			Unity::il2cppPropertyInfo* pProperty = reinterpret_cast<Unity::il2cppPropertyInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetPropertyFromName)(m_Object.m_pClass, m_pPropertyName);
			if (pProperty && pProperty->m_pGet)
				return reinterpret_cast<T(UNITY_CALLING_CONVENTION)(void*)>(pProperty->m_pGet->m_pMethodPointer)(GetManagedObjectPointer());

			T tDefault = {};
			return tDefault;
		}

		template<typename T>
		void SetPropertyValue(const char* m_pPropertyName, T m_tValue)
		{
			if (!Functions.m_ClassGetPropertyFromName || !m_Object.m_pClass || !m_pPropertyName)
				return;

			Unity::il2cppPropertyInfo* pProperty = reinterpret_cast<Unity::il2cppPropertyInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetPropertyFromName)(m_Object.m_pClass, m_pPropertyName);
			if (pProperty && pProperty->m_pSet)
				reinterpret_cast<void(UNITY_CALLING_CONVENTION)(void*, T)>(pProperty->m_pSet->m_pMethodPointer)(GetManagedObjectPointer(), m_tValue);
		}

		template<typename T>
		__forceinline T GetMemberValue(int m_iOffset)
		{
			return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
		}

		template<typename T>
		__forceinline void SetMemberValue(int m_iOffset, T m_tValue)
		{
			*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset) = m_tValue;
		}

		template<typename T>
		__forceinline T GetMemberValue(Unity::il2cppFieldInfo* m_pField)
		{
			if (!m_pField || m_pField->m_iOffset < 0)
			{
				T m_tDefault = {};
				return m_tDefault;
			}

			return GetMemberValue<T>(m_pField->m_iOffset);
		}

		template<typename T>
		__forceinline void SetMemberValue(Unity::il2cppFieldInfo* m_pField, T m_tValue)
		{
			if (!m_pField || m_pField->m_iOffset < 0)
				return;

			SetMemberValue<T>(m_pField->m_iOffset, m_tValue);
		}

		template<typename T>
		T GetMemberValue(const char* m_pMemberName)
		{
			if (!Functions.m_ClassGetFieldFromName || !m_Object.m_pClass || !m_pMemberName)
			{
				T tDefault = {};
				return tDefault;
			}

			Unity::il2cppFieldInfo* pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_Object.m_pClass, m_pMemberName);
			if (pField)
			{
				if (pField->m_iOffset >= 0)
					return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + pField->m_iOffset);
			}
			else
			{
				return GetPropertyValue<T>(m_pMemberName);
			}

			T tDefault = {};
			return tDefault;
		}

		template<typename T>
		void SetMemberValue(const char* m_pMemberName, T m_tValue)
		{
			if (!Functions.m_ClassGetFieldFromName || !m_Object.m_pClass || !m_pMemberName)
				return;

			Unity::il2cppFieldInfo* pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_Object.m_pClass, m_pMemberName);
			if (pField)
			{
				if (pField->m_iOffset >= 0)
					*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + pField->m_iOffset) = m_tValue;
				return;
			}

			SetPropertyValue<T>(m_pMemberName, m_tValue);
		}

		template<typename T>
		__forceinline T GetObscuredViaOffset(int m_iOffset)
		{
			if (m_iOffset >= 0)
			{
				switch (sizeof(T))
				{
				case sizeof(double):
				{
					long long m_lKey = *reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
					long long m_lValue = *reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset + sizeof(m_lKey));

					m_lValue ^= m_lKey;
					return *reinterpret_cast<T*>(&m_lValue);
				}
				case sizeof(int):
				{
					int m_iKey = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
					int m_iValue = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset + sizeof(m_iKey));

					m_iValue ^= m_iKey;
					return *reinterpret_cast<T*>(&m_iValue);
				}
				case sizeof(bool):
				{
					unsigned char m_uKey = *reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
					int m_iValue = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset + sizeof(m_uKey));

					m_iValue ^= m_uKey;
					return *reinterpret_cast<T*>(&m_iValue);
				}
				default:
					break;
				}
			}

			T m_tDefault = {};
			return m_tDefault;
		}

		template<typename T>
		T GetObscuredValue(const char* m_pMemberName)
		{
			if (!Functions.m_ClassGetFieldFromName || !m_Object.m_pClass || !m_pMemberName)
			{
				T m_tDefault = {};
				return m_tDefault;
			}

			Unity::il2cppFieldInfo* m_pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_Object.m_pClass, m_pMemberName);
			return GetObscuredViaOffset<T>(m_pField ? m_pField->m_iOffset : -1);
		}

		template<typename T>
		__forceinline void SetObscuredViaOffset(int m_iOffset, T m_tValue)
		{
			if (m_iOffset < 0)
				return;

			switch (sizeof(T))
			{
			case sizeof(double):
			{
				long long m_lKey = *reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
				long long* m_pValue = reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset + sizeof(m_lKey));
				*m_pValue = *reinterpret_cast<long long*>(&m_tValue) ^ m_lKey;
				break;
			}
			case sizeof(int):
			{
				int m_iKey = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
				int* m_pValue = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset + sizeof(m_iKey));
				*m_pValue = *reinterpret_cast<int*>(&m_tValue) ^ m_iKey;
				break;
			}
			case sizeof(bool):
			{
				unsigned char m_uKey = *reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset);
				int* m_pValue = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GetManagedObjectPointer()) + m_iOffset + sizeof(m_uKey));
				*m_pValue = *reinterpret_cast<int*>(&m_tValue) ^ m_uKey;
				break;
			}
			default:
				break;
			}
		}

		template<typename T>
		void SetObscuredValue(const char* m_pMemberName, T m_tValue)
		{
			if (!Functions.m_ClassGetFieldFromName || !m_Object.m_pClass || !m_pMemberName)
				return;

			Unity::il2cppFieldInfo* m_pField = reinterpret_cast<Unity::il2cppFieldInfo * (IL2CPP_CALLING_CONVENTION)(void*, const char*)>(Functions.m_ClassGetFieldFromName)(m_Object.m_pClass, m_pMemberName);
			if (!m_pField)
				return;

			SetObscuredViaOffset<T>(m_pField->m_iOffset, m_tValue);
		}
	};
}
