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

## Usage


```
struct MyStruct
{
  float f{};
  std::vector<float> vf;
};

MyStruct s;

// Register handler.
staticstruct::ObjectHandler h;
h.add_property("f", &s.f);
h.add_property("vf", &s.vf);

// Write parser logic.
// `ParseStruct` will iterate over registered properties and call the callback function(lambda).
// Application need to explicitly specify how to set value.
//
// You can use `staticstruct::ParseUtil::SetValue` to set value easily.
//
staticstruct::Reader r;
bool ret = r.ParseStruct(&h, [](std::string key, uint32_t flags, staticstruct::BaseHandler &handler) -> bool {
  if (key == "f") {
    return staticstruct::ParseUtil::SetValue(1.0f, handler);
    // or you can do this: return handler.Float(1.0f);
  } else if (key == "vf") {
    std::vector<float> data = {1.0f, 2.0f, 3.3f, 4.5f};
    return staticstruct::ParseUtil::SetValue(data, handler);
  }
  std::cerr << "Need to implement how to parse property `" + key + "`\n";
  return false;
});

if (!ret) {
  std::cerr << "failed\n";
  return -1;
}
```

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

* [ ] Specialize frequently used datatype
  * [x] `std::vector<float>`
  * [ ] `std::vector<std::array<float, 3>>` (float3)
  * [ ] `std::vector<std::array<float, 4>>` (float4)
  * [ ] `std::vector<std::array<float, 16>>` (matrix4x4)
* [ ] Better error report.
* [ ] Refator code.
* [ ] Implement schema generator.
* [ ] Serializer.
* [ ] Unit tests.

## License

MIT license.

### Third party license

* StaticJSON: MIT license
