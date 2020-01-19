#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  std::vector<unsigned char> myData;
  for (unsigned i = 65; i < 91; ++i) {
    myData.push_back(i);
  }

  for (unsigned int i = 0; i < myData.size(); ++i) {
    std::cout << myData[i];
  }

  return 0;
}