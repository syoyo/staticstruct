#include "staticstruct.hh"

#include <iostream>
#include <cassert>

struct MyStruct
{
  float f{};
  std::vector<float> vf;
  std::vector<std::array<float, 3>> vf3;
  std::string name;
  std::string uuid;
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
