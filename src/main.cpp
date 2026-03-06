#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "\n  [\033[1;31mERROR\033[0m]: ";
    std::cout << "No filepath received, exiting program.\n\n";
    std::cout << "  \033[1mUsage:\033[0m ";
    std::cout << "decode8086 <binary filepath>\n\n";

    return 1;
  }

  if (argc == 2 && std::string(argv[1]) == "--help") {
    std::cout << "\n  \033[1m\033[4m8086 Decoder\033[0m\n\n";
    std::cout << "  \033[1mUsage:\033[0m ";
    std::cout << "decode8086 <binary filepath>\n\n";

    return 0;
  }

  if (argc > 2) {
    std::cout << "\n  [\033[1;31mERROR\033[0m]: ";
    std::cout
        << "The decoder currently only supports one filepath at a time.\n\n";

    return 1;
  }

  std::cout << argc << ", " << argv << "\n";

  return 0;
}
