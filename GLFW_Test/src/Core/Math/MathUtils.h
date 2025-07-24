#pragma once
#include <type_traits>

namespace MathUtil {

	template<typename A, typename B, typename T>
	auto Lerp(A a, B b, T t) {
		static_assert(
			std::is_arithmetic<A>::value &&
			std::is_arithmetic<B>::value &&
			std::is_arithmetic<T>::value,
			"Lerp requires arithmetic types"
			);

		using Result = std::common_type_t<A, B, T>;
		return static_cast<Result>(a) + (static_cast<Result>(b) - static_cast<Result>(a)) * static_cast<Result>(t);
	}

	template<typename A, typename B>
	auto Min(A a, B b) {
		static_assert(std::is_arithmetic<A>::value && std::is_arithmetic<B>::value,
			"Min requires arithmetic types");

		using Result = std::common_type_t<A, B>;
		return (a > b) ? static_cast<Result>(b) : static_cast<Result>(a);
	}

	template<typename A, typename B>
	auto Max(A a, B b) {
		static_assert(std::is_arithmetic<A>::value && std::is_arithmetic<B>::value,
			"Max requires arithmetic types");

		using Result = std::common_type_t<A, B>;
		return (a > b) ? static_cast<Result>(a) : static_cast<Result>(b);
	}

}
