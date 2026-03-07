#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string_view>

void display_success_badge() { printf("\n  [\033[1;32mSUCCESS\033[0m]: "); }
void display_error_badge() { printf("\n  [\033[1;31mERROR\033[0m]: "); }
void display_usage_instruction() {
  printf("  \033[1mUsage:\033[0m ");
  printf("decode8086 <binary filepath>\n\n");
}

int main(int argc, char *argv[]) {
  bool help_requested = argc == 2 && std::string_view(argv[1]) == "--help";
  bool no_args = argc < 2;
  bool multi_args = argc > 2;

  if (help_requested) {
    printf("\n  \033[1m\033[4m8086 Decoder\033[0m\n\n");
    display_usage_instruction();

    return 0;
  }

  if (no_args) {
    display_error_badge();
    printf("No filepath received, exiting program.\n\n");
    display_usage_instruction();

    return 1;
  }

  if (multi_args) {
    display_error_badge();
    printf("The decoder currently only supports one filepath at a time.\n\n");

    return 1;
  }

  FILE *f = fopen(argv[1], "rb");

  if (!f) {
    display_error_badge();
    printf("Could not open file\n\n");

    return 1;
  }

  fseek(f, 0, SEEK_END);
  size_t filesize = ftell(f);
  rewind(f);

  uint8_t buf[4096];
  constexpr size_t buf_size = sizeof(buf);
  size_t total_bytes_read{0};

  while (true) {
    const size_t bytes_read = fread(buf, 1, buf_size, f);
    total_bytes_read += bytes_read;

    for (size_t i{0}; i < bytes_read; ++i) {
      if (i % 8 == 0)
        printf("\n  ");
      else if (i % 4 == 0)
        printf(" ");

      printf("%02x ", buf[i]);
    }

    if (bytes_read < buf_size) {
      if (ferror(f)) {
        display_error_badge();
        printf("read error\n\n");
        return 1;
      }
      if (feof(f)) {
        printf("\n");
        display_success_badge();
        printf("Finished reading file.\n");
        break;
      }
    }
  }

  fclose(f);

  printf("\n  filesize: %zu\n  total_bytes_read: %zu\n\n", filesize,
         total_bytes_read);

  return 0;
}
