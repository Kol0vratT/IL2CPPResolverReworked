#pragma once

namespace IL2CPP
{
    namespace Utils
    {
        namespace VTable
        {
            inline void ReplaceFunction(void** m_VTableFunc, void* m_NewFunc, void** m_Original = nullptr)
            {
                if (!m_VTableFunc || !m_NewFunc)
                    return;

                DWORD m_OldProtection = 0x0;
                if (!VirtualProtect(m_VTableFunc, sizeof(void*), PAGE_READWRITE, &m_OldProtection))
                    return;

                if (m_Original)
                    *m_Original = *m_VTableFunc;

                *m_VTableFunc = m_NewFunc;
                VirtualProtect(m_VTableFunc, sizeof(void*), m_OldProtection, &m_OldProtection);
                FlushInstructionCache(GetCurrentProcess(), m_VTableFunc, sizeof(void*));
            }

            inline void** FindFunction(void** m_VTable, int m_Count, std::initializer_list<unsigned char> m_Opcodes)
            {
                if (!m_VTable || m_Count <= 0 || m_Opcodes.size() == 0)
                    return nullptr;

                size_t m_OpcodeSize = m_Opcodes.size();
                const void* m_OpcodesPtr = m_Opcodes.begin();

                for (int i = 0; m_Count > i; ++i)
                {
                    if (!m_VTable[i])
                        continue;

                    if (memcmp(m_VTable[i], m_OpcodesPtr, m_OpcodeSize) == 0)
                        return &m_VTable[i];
                }

                return nullptr;
            }

            // Pattern+mask matcher (mask byte 0xFF = match, 0x00 = wildcard)
            inline void** FindFunctionMasked(void** m_VTable, int m_Count, const unsigned char* pattern, const unsigned char* mask, size_t patternSize)
            {
                if (!m_VTable || !pattern || !mask || patternSize == 0)
                    return nullptr;

                for (int i = 0; i < m_Count; ++i)
                {
                    void* p = m_VTable[i];
                    if (!p)
                        continue;

                    bool ok = true;
                    for (size_t b = 0; b < patternSize; ++b)
                    {
                        if (mask[b] && reinterpret_cast<unsigned char*>(p)[b] != pattern[b])
                        {
                            ok = false;
                            break;
                        }
                    }
                    if (ok)
                        return &m_VTable[i];
                }

                return nullptr;
            }
        }
    }
}
