#pragma once

namespace IL2CPP
{
	struct CallbackHook_t
	{
		std::vector<void*> m_Funcs;

		void** m_VFunc = nullptr;
		void* m_Original = nullptr;
	};

	namespace Callback
	{
		namespace OnUpdate
		{
			CallbackHook_t m_CallbackHook;

			void Add(void* m_pFunction)
			{
				m_CallbackHook.m_Funcs.emplace_back(m_pFunction);
			}

			void __fastcall Hook(void* rcx)
			{
				for (void* m_Func : m_CallbackHook.m_Funcs)
					reinterpret_cast<void(*)()>(m_Func)();

				reinterpret_cast<void(__fastcall*)(void*)>(m_CallbackHook.m_Original)(rcx);
			}
		}

		namespace OnLateUpdate
		{
			CallbackHook_t m_CallbackHook;

			void Add(void* m_pFunction)
			{
				m_CallbackHook.m_Funcs.emplace_back(m_pFunction);
			}

			void __fastcall Hook(void* rcx)
			{
				for (void* m_Func : m_CallbackHook.m_Funcs)
					reinterpret_cast<void(*)()>(m_Func)();

				reinterpret_cast<void(__fastcall*)(void*)>(m_CallbackHook.m_Original)(rcx);
			}
		}

		void Initialize()
		{
			void* m_IL2CPPThread = Thread::Attach(IL2CPP::Domain::Get());

			Unity::CComponent* mb = IL2CPP::Helper::GetMonoBehaviour();
			if (!mb || !mb->m_CachedPtr)
			{
				IL2CPP::Thread::Detach(m_IL2CPPThread);
				return;
			}

			void** m_MonoBehaviourVTable = *reinterpret_cast<void***>(mb->m_CachedPtr);
			if (m_MonoBehaviourVTable)
			{
#ifdef _WIN64
				// Unity versions may slightly change thunk prologues; use masked patterns + scan a wider range.
				const unsigned char upd_pat1[] = { 0x33, 0xD2, 0xE9 }; // xor edx, edx | jmp
				const unsigned char upd_msk1[] = { 0xFF, 0xFF, 0xFF };

				const unsigned char late_pat1[] = { 0xBA, 0x01, 0x00, 0x00, 0x00, 0xE9 }; // mov edx,1 | jmp
				const unsigned char late_msk1[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

				OnUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunctionMasked(m_MonoBehaviourVTable, 256, upd_pat1, upd_msk1, sizeof(upd_pat1));
				OnLateUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunctionMasked(m_MonoBehaviourVTable, 256, late_pat1, late_msk1, sizeof(late_pat1));

				// Fallback: original exact matcher (some titles still match)
				if (!OnUpdate::m_CallbackHook.m_VFunc)
					OnUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(m_MonoBehaviourVTable, 256, { 0x33, 0xD2, 0xE9 });
				if (!OnLateUpdate::m_CallbackHook.m_VFunc)
					OnLateUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(m_MonoBehaviourVTable, 256, { 0xBA, 0x01, 0x00, 0x00, 0x00, 0xE9 });
#elif _WIN32
				OnUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(m_MonoBehaviourVTable, 512, { 0x6A, 0x00, 0xE8 }); // push 00 | call
				OnLateUpdate::m_CallbackHook.m_VFunc = Utils::VTable::FindFunction(m_MonoBehaviourVTable, 512, { 0x6A, 0x01, 0xE8 }); // push 01 | call
#endif
			}

			IL2CPP::Thread::Detach(m_IL2CPPThread);

			// Replace (Hook) - only if we successfully located targets
			if (OnUpdate::m_CallbackHook.m_VFunc)
				Utils::VTable::ReplaceFunction(OnUpdate::m_CallbackHook.m_VFunc, OnUpdate::Hook, &OnUpdate::m_CallbackHook.m_Original);
			if (OnLateUpdate::m_CallbackHook.m_VFunc)
				Utils::VTable::ReplaceFunction(OnLateUpdate::m_CallbackHook.m_VFunc, OnLateUpdate::Hook, &OnLateUpdate::m_CallbackHook.m_Original);
		}

		void Uninitialize()
		{
			Utils::VTable::ReplaceFunction(OnUpdate::m_CallbackHook.m_VFunc, OnUpdate::m_CallbackHook.m_Original);
			Utils::VTable::ReplaceFunction(OnLateUpdate::m_CallbackHook.m_VFunc, OnLateUpdate::m_CallbackHook.m_Original);
		}
	}
}
