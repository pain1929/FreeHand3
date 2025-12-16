#pragma once

#define STB_OMIT_TESTS
#include "pch.h"
#include <MinHook.h>
#include <stdexcept>
namespace Memory
{
	// Checks to see if memory is readable to prevent access violations, can be expensive
	bool IsReadable(const void* lpAddress, size_t dwLength);


	inline void* GetVirtualMethod(void* lpAddress, size_t index)
	{
		if (!lpAddress)
			return nullptr;

		void** pMethodTable = *static_cast<void***>(lpAddress);
		if (!pMethodTable)
			return nullptr;

		void* pMethod = pMethodTable[index];
		return pMethod ? pMethod : nullptr;
	}


	template<typename TFunction>
	class Hook
	{
	private:
		void* m_pTarget = nullptr;
		TFunction m_fnOriginal = nullptr;

	public:
		Hook() {};

		// Call the original function that we hooked
		template <typename... TArgs>
		auto operator()(TArgs... args)
		{
			static_assert(std::is_invocable_v<TFunction, TArgs...>, "Args dont match function type!");
			if (!m_fnOriginal)
				throw std::runtime_error("Attempted to call a nullptr!");

			if (!m_pTarget)
				return std::invoke_result_t<TFunction, TArgs...>{};

			return m_fnOriginal(std::forward<TArgs>(args)...);
		};

		// Disable the already installed hook (wont remove it)
		MH_STATUS Disable()
		{
			return MH_DisableHook(m_pTarget);
		};

		// Enable the already installed hook
		MH_STATUS Enable()
		{
			return MH_EnableHook(m_pTarget);
		};

		// Remove the installed hook
		MH_STATUS RemoveHook()
		{
			MH_STATUS status = Disable();

			if (status != MH_OK && status != MH_ERROR_DISABLED)
				return status;

			status = MH_RemoveHook(m_pTarget);
			if (status == MH_OK)
				m_pTarget = nullptr;

			return status;
		};

		// Hook function via a function pointer
		MH_STATUS HookAddress(TFunction fnHook, void* pTarget)
		{
			if (MH_STATUS status = MH_CreateHook(pTarget, fnHook, reinterpret_cast<void**>(&m_fnOriginal)); status != MH_OK)
				return status;

			m_pTarget = pTarget;
			return Enable();
		};

		// Hook the function via a virtual method table index
		MH_STATUS HookVirtualMethod(TFunction fnHook, void* lpAddress, size_t index)
		{
			return HookAddress(fnHook, GetVirtualMethod(lpAddress, index));
		}
	};
}