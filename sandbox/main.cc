#include <type_traits>
#include <iostream>
#include <vector>

template<typename T>
struct Traits;

template<>
struct Traits<float>
{
  static constexpr auto type_name = "float";
};

template<class T>
class Value
{
 private:
  T m_value;

 public:
  T value() const {
    return m_value;
  }

  template<class U>
  bool assign(const U &u) {
    if (!std::is_same<T, U>::value) {
      return false;
    }

    m_value = u;

    return true;
  }

  std::string type_name() {
    return std::string(Traits<T>::type_name);
  }
};

template<class T>
class Value<std::vector<T>>
{
 private:
  T m_value;

 public:

  std::string type_name() {
    return "std::vector<" + std::string(Traits<T>::type_name) + ">";
  }
};

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  Value<float> f;
  Value<std::vector<float>> vf;

  float x = 3.2f;
  //int y = 2;

  std::cout << Traits<float>::type_name << "\n";
  std::cout << f.type_name() << "\n";
  std::cout << vf.type_name() << "\n";
  std::cout << vf.type_name() << "\n";

  std::cout << f.assign(x) << "\n";
  std::cout << f.value() << "\n";


  return 0;

}
