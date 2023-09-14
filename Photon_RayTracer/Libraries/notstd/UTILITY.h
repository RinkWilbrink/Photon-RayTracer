#pragma once

namespace RinkWilbrink
{
    namespace notstd
    {
		template <class T>
		constexpr T move(T&& _Arg) noexcept
		{
			return static_cast<T&&>(_Arg);
		}
    }
}