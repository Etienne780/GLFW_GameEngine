#pragma once
#include <type_traits>

class MathUtil {
public:
	template<typename A, typename B, typename T>
	static auto Lerp(A a, B b, T t) {
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
	static auto Min(A a, B b) {
		static_assert(std::is_arithmetic<A>::value && std::is_arithmetic<B>::value,
			"Min requires arithmetic types");

		using Result = std::common_type_t<A, B>;
		return (a > b) ? static_cast<Result>(b) : static_cast<Result>(a);
	}

	template<typename A, typename B>
	static auto Max(A a, B b) {
		static_assert(std::is_arithmetic<A>::value && std::is_arithmetic<B>::value,
			"Max requires arithmetic types");

		using Result = std::common_type_t<A, B>;
		return (a > b) ? static_cast<Result>(a) : static_cast<Result>(b);
	}

	template<typename A, typename B, typename C>
	static auto Clamp(A a, B min, C max) {
		static_assert(std::is_arithmetic<A>::value &&
			std::is_arithmetic<B>::value &&
			std::is_arithmetic<C>::value,
			"Clamp requires arithmetic types");

		if (min > max)
			std::swap(min, max);
		
		using Result = std::common_type_t<A, B, C>;
		Result result;
		if (a < min) {
			result = min;
		}
		else if (a > max) {
			result = max;
		}
		else {
			result = a;
		}
		return result;
	}

	template<typename A, typename B, typename C>
	static void Clamp(A& a, B min, C max) {
		static_assert(std::is_arithmetic<A>::value &&
			std::is_arithmetic<B>::value &&
			std::is_arithmetic<C>::value,
			"Clamp requires arithmetic types");

		if (min > max)
			std::swap(min, max);

		if (a < min) {
			a = static_cast<A>(min);
		}
		else if (a > max) {
			a = static_cast<A>(max);
		}
	}

private:
	MathUtil();
};