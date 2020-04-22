#include <iostream>
#include <functional>

using namespace std;

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
