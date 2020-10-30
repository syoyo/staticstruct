# staticstruct, Simple statically typed struct serialization/deserialization library in C++11

`staticstruct` is a simple, single `.hh/.cc` file C++11 library to reconstruct/save statically typed C++ struct.

When we write graphics and machine learning applications, there are many situation that we need to operate with statically-typed, predefined struct data. And we want to serialize/deserialize such data into/from binary and ASCII format(e.g. JSON).

`staticstruct` help to write such converters/adapters.

Existing serialization library in C++11 is too complex and too generic. In most cases simple basic types and limited support of STL container types are suffice.

`staticstruct` is based on StaticJSON: https://github.com/netheril96/StaticJSON

## Status

Work in progres. currently some deserialization features are available.

## Requirements

* C++11 compiler
  * clang, gcc, MSVC, ...

## How to build

Simply copy `staticstruct.cc` and `staticstruct.hh` to your project.

## Supported basic types

* bool
* short, ushort
* int, uint(int32, uint32)
* int64, uintt64
* float, double
* std::string

## Supported STL containers

* `std::map`
* `std::unordered_map`
* `std::list`
* `std::vector`
* `std::array`

## TODO

* [ ] Better error report.
* [ ] Refator code.
* [ ] Implement schema generator.
* [ ] Serializer.

## License

MIT license.

### Third party license

* StaticJSON: MIT license
