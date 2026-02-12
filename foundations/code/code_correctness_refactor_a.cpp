#include <iostream>
#include <vector>

int main() {
  std::vector<int> v;
  v.reserve(50);

  for (signed int i = 0; i < 50; ++i) {
    v.push_back(i);
  }

  for (signed int i = v.size() - 1; i >= 0; --i) {
    std::cout << i << ": " << v[i] << std::endl;
  }
  return 0;
}
