#include <iostream>
using namespace std;

class C {
public:
  int value;
};

void combine(const C& a, const C& b, C* out) {
  out->value = a.value + b.value;
}

int main(int argc, char ** argv) {
  C source_1{2};
  C source_2{4};
  C dest;

  combine(source_1, source_2, &dest);
  cout << dest.value << endl;
  
  return 0;
}
