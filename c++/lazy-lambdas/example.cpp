#include <iostream>
using namespace std;

class C {
public:
  int value;
};

void combine(const C& a, const C& b, double mix, C* out) {
  if (mix == 1.0) {
    out->value = a.value;
    return;
  }

  if (mix == 0.0) {
    out->value = b.value;
    return;
  }

  out->value = (mix * a.value) + ((1.0 - mix) * b.value);
}

int main(int argc, char ** argv) {
  C source_1{2};
  C source_2{4};
  C dest;

  combine(source_1, source_2, 0.8, &dest);
  cout << dest.value << endl;
  
  return 0;
}
