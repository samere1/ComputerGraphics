#include <iostream>
#include <string>

int main() {
  std::string fullName = "Samer Elkheir";
  std::string firstName = fullName.substr(0, 5);
  std::string lastName = fullName.substr(6, 7);

  std::cout << "First: " << firstName << std::endl;
  std::cout << "Last: " << lastName << std::endl;

  unsigned int i = 0;
  while (i < fullName.length()) {
    if (fullName.at(i) == ' ') break;
    std::cout << fullName[i++];
  }

  return 0;
}