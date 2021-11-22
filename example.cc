#include "staticstruct.hh"

#include <iostream>
#include <cassert>
#include <cstring>

// custom types
struct matrix4
{
  float m[4][4];
};

namespace staticstruct
{
template <>
struct Converter<matrix4>
{
    // Raw float[4][4] is not assignable, so use flattened 1D std::array.
    typedef std::array<float, 16> shadow_type;
    // This typedef is a must. The shadow type is a C++ type
    // that can be directly converted to and from JSON values.

    static_assert(sizeof(shadow_type) == sizeof(float) * 4 * 4, "sizeof mismatch");

    static std::unique_ptr<Error> from_shadow(const shadow_type& shadow, matrix4& value)
    {
        memcpy(value.m, &shadow[0], sizeof(float) * 4 * 4);

        return nullptr; // success
    }

    static void to_shadow(const matrix4& value, shadow_type& shadow)
    {
        memcpy(&shadow[0], value.m, sizeof(shadow_type));
    }
};
}

struct MyStruct
{
  float f{};
  std::vector<float> vf;
  std::vector<std::array<float, 3>> vf3;
  std::string name;
  std::string uuid;
  matrix4 mtx;
};

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  MyStruct s;

  // Register handler.
  {
    staticstruct::ObjectHandler h;
    h.add_property("f", &s.f);
    h.add_property("vf", &s.vf);
    h.add_property("name", &s.name);
    h.add_property("uuid", &s.uuid, staticstruct::Flags::Optional);
    h.add_property("vf3", &s.vf3, staticstruct::Flags::Optional);
    h.add_property("mtx", &s.mtx, staticstruct::Flags::Optional);

    // Write parser logic.
    // `ParseStruct` will iterate over registered properties and call the callback function(lambda).
    // Application need to explicitly specify how to set value.
    staticstruct::Reader r;
    std::string err;
    bool ret = r.ParseStruct(&h, [](std::string key, uint32_t flags, staticstruct::BaseHandler &handler) -> bool {
      (void)flags;

      if (key == "f") {
        return staticstruct::ParseUtil::SetValue(1.0f, handler);
        // or you can do this: return handler.Float(1.0f);
      } else if (key == "vf") {
        std::vector<float> data = {1.0f, 2.0f, 3.3f, 4.5f};
        return staticstruct::ParseUtil::SetValue(data, handler);
      } else if (key == "name") {
        return staticstruct::ParseUtil::SetValue("muda", handler);
      } else if (key == "vf3") {
        std::vector<std::array<float, 3>> data = {{1.0f, 2.0f, 3.3f}, {4.5f, 6.3f, 7.4f}};
        return staticstruct::ParseUtil::SetValue(data, handler);
      } else if (key == "mtx") {
        matrix4 m;

        m.m[0][0] = 1.0;
        m.m[0][1] = 0.0;
        m.m[0][2] = 0.0;
        m.m[0][3] = 0.0;

        m.m[1][0] = 0.0;
        m.m[1][1] = 1.0;
        m.m[1][2] = 0.0;
        m.m[1][3] = 0.0;

        m.m[2][0] = 0.0;
        m.m[2][1] = 0.0;
        m.m[2][2] = 1.0;
        m.m[2][3] = 0.0;

        m.m[3][0] = 0.0;
        m.m[3][1] = 0.0;
        m.m[3][2] = 0.0;
        m.m[3][3] = 1.0;

        staticstruct::Handler<matrix4> h(&m);

        return h.write(&handler);
      }

      if (flags & staticstruct::Flags::Optional) {
        std::cout << "Optional property `" + key + "`. OK to return false.\n";
      } else {
        std::cerr << "Need to implement how to parse property `" + key + "`\n";
      }

      return false;
    }, &err);

    if (!ret) {
      std::cerr << "failed\n";
      return -1;
    }

    std::cout << "MyStruct.f = " << s.f << "\n";
    std::cout << "MyStruct.name = `" << s.name << "`\n";
    for (size_t i = 0; i < s.vf.size(); i++) {
      std::cout << "MyStruct.vf[" << i << "] = " << s.vf[i] << "\n";
    }
    for (size_t i = 0; i < s.vf3.size(); i++) {
      std::cout << "MyStruct.vf3[" << i << "] = " << s.vf3[i][0] << ", " << s.vf3[i][1] << ", " << s.vf3[i][2] << "\n";
    }

    std::cout << "MyStruct.mtx = [[" << s.mtx.m[0][0] << ", " << s.mtx.m[0][1] << ", " << s.mtx.m[0][2] << ", " << s.mtx.m[0][3] << "]\n";
    std::cout <<  " [" << s.mtx.m[1][0] << ", " << s.mtx.m[1][1] << ", " << s.mtx.m[1][2] << ", " << s.mtx.m[1][3] << "]\n";
    std::cout <<  " [" << s.mtx.m[2][0] << ", " << s.mtx.m[2][1] << ", " << s.mtx.m[2][2] << ", " << s.mtx.m[2][3] << "]\n";
    std::cout <<  " [" << s.mtx.m[3][0] << ", " << s.mtx.m[3][1] << ", " << s.mtx.m[3][2] << ", " << s.mtx.m[3][3] << "]]\n";

  }

  // Example code which results in type mismatch
  // TODO(syoyo): Get error message from Handler.
  {
    staticstruct::ObjectHandler h;
    h.add_property("f", &s.f);

    staticstruct::Reader r;
    std::string err;
    bool ret = r.ParseStruct(&h, [](std::string key, uint32_t flags, staticstruct::BaseHandler &handler) -> bool {
      (void)flags;

      if (key == "f") {
        // float expected but provide `const char *`. Will return false.
        bool ok = staticstruct::ParseUtil::SetValue("dora", handler);
        assert(ok == false);
        std::cout << "ret = " << ok << "\n";
        return ok;
      }

      return false;
    }, &err);


    if (!ret) {
      // expected.
      assert(err.size());
      if (err.size()) {
        std::cerr << "error message: " << err << "\n";
      }
    }
  }

  return 0;
}
