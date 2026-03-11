#pragma once

namespace IL2CPP
{
	namespace Thread
	{
		inline void* Attach(void* m_Domain)
		{
			if (!Functions.m_ThreadAttach)
				return nullptr;

			return reinterpret_cast<void* (IL2CPP_CALLING_CONVENTION)(void*)>(Functions.m_ThreadAttach)(m_Domain);
		}

		inline void Detach(void* m_Thread)
		{
			if (!Functions.m_ThreadDetach || !m_Thread)
				return;

			reinterpret_cast<void(IL2CPP_CALLING_CONVENTION)(void*)>(Functions.m_ThreadDetach)(m_Thread);
		}
	}

	class ScopedThreadAttachment
	{
	public:
		explicit ScopedThreadAttachment(void* m_Domain = nullptr)
		{
			if (!m_Domain)
				m_Domain = Domain::Get();

			m_Thread = Thread::Attach(m_Domain);
		}

		~ScopedThreadAttachment()
		{
			Thread::Detach(m_Thread);
		}

		ScopedThreadAttachment(const ScopedThreadAttachment&) = delete;
		ScopedThreadAttachment& operator=(const ScopedThreadAttachment&) = delete;

		inline void* Get() const
		{
			return m_Thread;
		}

		inline bool IsAttached() const
		{
			return m_Thread != nullptr;
		}

	private:
		void* m_Thread = nullptr;
	};

	namespace Thread
	{
		struct CreateThreadContext_t
		{
			void* m_OnStart = nullptr;
			void* m_OnEnd = nullptr;
		};

		inline DWORD WINAPI Handler(void* m_Reserved)
		{
			ScopedThreadAttachment m_Attachment;
			CreateThreadContext_t* m_Context = reinterpret_cast<CreateThreadContext_t*>(m_Reserved);
			if (!m_Context)
				return 0U;

			void* m_ThreadStart = m_Context->m_OnStart;
			void* m_ThreadEnd = m_Context->m_OnEnd;
			delete m_Context;

			reinterpret_cast<void(*)()>(m_ThreadStart)();
			if (m_ThreadEnd)
				reinterpret_cast<void(*)()>(m_ThreadEnd)();

			return 0U;
		}

		inline bool Create(void* m_OnStartFunc, void* m_OnEndFunc = nullptr)
		{
			IL2CPP_ASSERT(m_OnStartFunc && "IL2CPP::Thread::Create - m_OnStartFunc is nullptr");
			if (!m_OnStartFunc)
				return false;

			CreateThreadContext_t* m_Context = new (std::nothrow) CreateThreadContext_t;
			IL2CPP_ASSERT(m_Context && "IL2CPP::Thread::Create - Failed to allocate thread context");
			if (!m_Context)
			{
				return false;
			}

			m_Context->m_OnStart = m_OnStartFunc;
			m_Context->m_OnEnd = m_OnEndFunc;

			HANDLE m_Thread = CreateThread(nullptr, 0, Handler, m_Context, 0, nullptr);
			if (m_Thread)
			{
				CloseHandle(m_Thread);
				return true;
			}

			delete m_Context;
			IL2CPP_ASSERT(false && "IL2CPP::Thread::Create - CreateThread failed");
			return false;
		}
	}
}
