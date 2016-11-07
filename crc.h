/*
 * Compile time CRC32 library
 *
 * Copyright (c) 2016, Yohei Hase. All rights reserved.
 * This source code is licensed under the MIT license.
 */

#pragma once

#include <stdint.h>
#include <string>
#include <type_traits>

// simple crc32 implementation
class crc32_basic {
	// crc lookup table
	template <uint32_t Size>
	struct Table {
		template <uint32_t... i>
		struct indices_holder {};

		template <uint32_t N, uint32_t... i>
		struct make_indices : public std::conditional<(N >= 10), make_indices<N - 10, N - 10, N - 9, N - 8, N - 7, N - 6, N - 5, N - 4, N - 3, N - 2, N - 1, i...>, make_indices<N - 1, N - 1, i...>>::type {};

		template <uint32_t... i>
		struct make_indices<0, i...> {
			typedef indices_holder<i...> type;
		};

		typedef typename make_indices<Size>::type indices_type;

		uint32_t value[Size];

		constexpr Table(uint32_t poly) : Table(poly, indices_type()) {}

		template <uint32_t... index>
		constexpr Table(uint32_t poly, indices_holder<index...>) : value{ crc_table(index, poly)... }
		{
		}
	};

	static constexpr uint32_t crc_table_impl(uint32_t crc, uint32_t poly, uint8_t loop)
	{
		return loop > 0 ? crc_table_impl((crc & 1) ? (poly ^ (crc >> 1)) : (crc >> 1), poly, loop - 1) : crc;
	}

	static constexpr uint32_t crc_table(uint32_t index, uint32_t poly)
	{
		return crc_table_impl(index, poly, 8);
	}

	constexpr uint32_t crc32_impl(const char* str, size_t len, uint32_t index, uint32_t hash) const
	{
		return index < len ? crc32_impl(str, len, index + 1, table.value[(hash ^ str[index]) & 0xFF] ^ (hash >> 8)) : hash;
	}

	Table<256> table;

public:
	constexpr crc32_basic(uint32_t poly) : table(poly) {}

	constexpr uint32_t operator()(const char* str, size_t len) const
	{
		return crc32_impl(str, len, 0, 0xFFFFFFFF) ^ 0xFFFFFFFF;
	}

	// char array ver
	template <size_t N>
	constexpr uint32_t operator()(const char(&str)[N]) const
	{
		return operator()(str, N - 1);
	}

	// char* ver (runtime only)
	template <typename T>
	inline typename std::enable_if<std::is_convertible<T, const char*>::value, uint32_t>::type operator()(T str) const
	{
		return operator()(str, strlen(str));
	}

	// std::string ver (runtime only)
	template <template <class> class Traits, template <class> class Alloc>
	inline uint32_t operator()(const std::basic_string<char, Traits<char>, Alloc<char>>& str) const
	{
		return operator()(str.c_str(), str.length());
	}
};

constexpr crc32_basic crc32(0xEDB88320);
