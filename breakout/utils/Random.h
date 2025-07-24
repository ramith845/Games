#pragma once
#include <random>
#include <chrono>

class Random
{
public:
	Random() {}
	~Random() {}

	inline static int get(int min, int max)
	{
		return std::uniform_int_distribution{ min, max }(mt);
	}

	template <typename T>
	inline static T get(T min, T max)
	{
		return std::uniform_real_distribution<T>{min, max}(mt);
	}

	template <typename R, typename S, typename T>
	inline static R get(S min, T max)
	{
		return get<R>(static_cast<R>(min), static_cast<R>(max));
	}

private:
	inline static std::mt19937 generate()
	{
		std::random_device rd{};
		std::seed_seq ss{
			static_cast<std::mt19937::result_type>(
				std::chrono::steady_clock::now().time_since_epoch().count()),
			rd(),
			rd(),
			rd(),
			rd(),
			rd(),
			rd() };
		return std::mt19937{ ss };
	}

	inline static std::mt19937 mt{ generate() };
};
