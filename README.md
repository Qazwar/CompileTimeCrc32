# CompileTimeCrc32
This is a compile-time CRC32 Library in C++11.

* one header-file only (crc.h)
* compile-time calculation of string literal CRC32 value
* supported for char pointer and std::string in the same interface (runtime only)

## Usage
This library usage is very simple.

    uint32_t crc_value = crc32("aaa");

If you need any more information, please check the main.cpp file.

### Specific Polynomial
    constexpr crc32_basic mycrc32(0xEDB88320);
    constexpr uint32_t myvalue = mycrc32("aaa");

## License
CompileTimeCrc32 is licensed under the [MIT License](https://github.com/yhase7/lua_planner/blob/master/LICENSE).
