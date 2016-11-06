/*
 * Compile time CRC32 library
 *
 * Copyright (c) 2016, Yohei Hase. All rights reserved.
 * This source code is licensed under the MIT license.
 */

#include "crc.h"
#include <cstdio>
#include <cassert>

int main(int argc, char* argv[])
{
	static_assert(crc32("aaa") == 4027020077, "crc calculation error");

	std::string stdstr = "aaa";
	assert(crc32(stdstr) == crc32("aaa"));

	const char* charptr = "aaa";
	assert(crc32(charptr) == crc32("aaa"));

	if (argc >= 2)
	{
		switch (crc32(argv[1]))
		{
		case crc32("foo"):
			puts("foo");
			break;

		case crc32("bar"):
			puts("bar");
			break;

		default:
			printf("%u\n", crc32(argv[1]));
			break;
		}
	}

	return 0;
}
