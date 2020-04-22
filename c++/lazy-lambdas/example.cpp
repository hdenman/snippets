#include <iostream>
#include <functional>
#include <string_view>

using namespace std;

// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
template <typename T>
constexpr std::string_view type_name() {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "std::string_view type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr std::string_view type_name() [with T = ";
  suffix = "; std::string_view = std::basic_string_view<char>]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix =
      "class std::basic_string_view<char,struct std::char_traits<char> > "
      "__cdecl type_name<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

class C {
public:
  int value;
};

using getter = function<const C&(void)>;

void combine(getter get_a, getter get_b, double mix, C* out) {  
  if (mix == 1.0) {
    const auto& a = get_a();
    out->value = a.value;
    return;
  }

  if (mix == 0.0) {
    const auto& b = get_b();
    out->value = b.value;
    return;
  }

  const auto& a = get_a();
  const auto& b = get_b();
  out->value = (mix * a.value) + ((1.0 - mix) * b.value);
}

void combine(const C& a, const C& b, double mix, C* out) {
  auto lambda_1 = [&] {return a;};
  auto lambda_2 = [&]() -> const C& {return a;};
  cout << type_name<decltype(lambda_1)>() << endl;
  cout << type_name<decltype(lambda_2)>() << endl;
  combine([&]() -> const C& {return a;},
	  [&]() -> const C& {return b;},
	  mix,
	  out);
}

int main(int argc, char ** argv) {
  C source_1{2};
  C source_2{4};
  C dest;

  combine(source_1, source_2, 0.8, &dest);
  cout << dest.value << endl;
  
  return 0;
}
