#include <cstdio>
#include <string_view>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("\n  [\033[1;31mERROR\033[0m]: ");
    printf("No filepath received, exiting program.\n\n");
    printf("  \033[1mUsage:\033[0m ");
    printf("decode8086 <binary filepath>\n\n");

    return 1;
  }

  if (argc == 2 && std::string_view(argv[1]) == "--help") {
    printf("\n  \033[1m\033[4m8086 Decoder\033[0m\n\n");
    printf("  \033[1mUsage:\033[0m ");
    printf("decode8086 <binary filepath>\n\n");

    return 0;
  }

  if (argc > 2) {
    printf("\n  [\033[1;31mERROR\033[0m]: ");
    printf("The decoder currently only supports one filepath at a time.\n\n");

    return 1;
  }

  printf("%i, %s\n", argc, argv[1]);

  return 0;
}
