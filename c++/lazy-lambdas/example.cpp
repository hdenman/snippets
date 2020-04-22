#include <iostream>
#include <functional>
#include <string_view>

// https://www.reddit.com/r/cpp/comments/62k8b0/lambda_initialization_trick_add_syntactic_sugar/
// https://deviorel.wordpress.com/2015/01/27/obtaining-function-pointers-from-lambdas-in-c/
// https://stackoverflow.com/questions/18889028/a-positive-lambda-what-sorcery-is-this

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

// https://stackoverflow.com/questions/39173519/template-type-deduction-with-stdfunction/39182901#39182901
// https://stackoverflow.com/questions/41466082/how-to-convert-stdbind-or-lambda-to-stdfunction-in-deduced-context
template<class T>
struct AsFunction
    : public AsFunction<decltype(&T::operator())>
{};

template<class ReturnType, class... Args>
struct AsFunction<ReturnType(Args...)> {
  using type = std::function<ReturnType(Args...)>;
};

template<class ReturnType, class... Args>
struct AsFunction<ReturnType(*)(Args...)> {
  using type = std::function<ReturnType(Args...)>;
};


template<class Class, class ReturnType, class... Args>
struct AsFunction<ReturnType(Class::*)(Args...) const> {
  using type = std::function<ReturnType(Args...)>;
};

template<class Class, class ReturnType>
struct AsFunction<ReturnType(Class::*)() const> {
  using type = std::function<ReturnType()>;
};

template<class F>
auto toFunction( F f ) -> typename AsFunction<F>::type {
  return {f};
}


class C {
public:
  int value;
  // C(const C&) = delete;
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

// https://shaharmike.com/cpp/lambdas-and-functions/

void combine(const C& a, const C& b, double mix, C* out) {
  auto lambda_1 = [&] {return a;};
  auto lambda_2 = [&]() -> const C& {return a;};
  cout << type_name<decltype(lambda_1)>() << endl;
  cout << type_name<decltype(lambda_2)>() << endl;

  cout << type_name<decltype(toFunction(lambda_1))>() << endl;
  cout << type_name<decltype(toFunction(lambda_2))>() << endl;

  
  combine([&]() -> const C& {return a;},
	  [&]() -> const C& {return b;},
	  mix,
	  out);
}

int main(int argc, char ** argv) {
  C source_1{2};
  C source_2{4};
  C dest{0};

  combine(source_1, source_2, 0.8, &dest);
  cout << dest.value << endl;
  
  return 0;
}
