#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string_view>

void display_success_badge() { printf("\n[\033[1;32mSUCCESS\033[0m]: "); }
void display_error_badge() { printf("\n[\033[1;31mERROR\033[0m]: "); }
void display_usage_instruction() {
  printf("\033[1mUsage:\033[0m ");
  printf("decode8086 <binary filepath>\n\n");
}

bool isMov(uint8_t byte) { return (byte & 0b11111100) == 0b10001000; }
bool isRegToReg(uint8_t byte) { return (byte & 0b11000000) == 0b11000000; }
bool isByteOp(uint8_t byte) { return (byte & 0b00000001) == 0; }
bool isRegSource(uint8_t byte) { return (byte & 0b00000010) == 0; }

int main(int argc, char *argv[]) {
  bool help_requested = argc == 2 && std::string_view(argv[1]) == "--help";
  bool no_args = argc < 2;
  bool multi_args = argc > 2;

  if (help_requested) {
    printf("\n\033[1m\033[4m8086 Decoder\033[0m\n\n");
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

  const char *name = strrchr(argv[1], '/');
  if (!name)
    name = strrchr(argv[1], '\\');
  name = name ? name + 1 : argv[1];
  printf("; %s disassembly:\n\nbits 16\n\n", name);

  uint8_t buf[4096]{};
  constexpr size_t buf_size = sizeof(buf);
  size_t total_bytes_read{0};

  while (true) {
    const size_t bytes_read = fread(buf, 1, buf_size, f);
    total_bytes_read += bytes_read;

    size_t cursor{0};
    while (cursor < bytes_read) {
      const uint8_t byte = buf[cursor];
      cursor++;

      // trying to read next_byte is UB
      if (cursor >= bytes_read) {
        break;
      }
      char instruction[64]{};
      // TODO: Implement a write utility using a cursor for better
      // performance, strcat scans the instruction for `\0` everytime it is
      // called which is unnecessary overhead + can cause buffer overflow,
      // although safe in the current implementation
      if (isMov(byte)) {
        strcat(instruction, "mov ");

        const uint8_t next_byte = buf[cursor];
        cursor++;

        const char *byte_reg[8]{"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
        const char *word_reg[8]{"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

        uint8_t reg_index = (next_byte & 0b00111000) >> 3;
        uint8_t r_m_index = next_byte & 0b00000111;

        uint8_t mod = (next_byte & 0b11000000) >> 6;

        switch (mod) {
        case 0:
          // memory mode, no displacement
        case 1:
          // memory mode, 8-bit displacement
        case 2:
          // memory mode, 16-bit displacement
        case 3:
          // register to register
          const char *(*register_table)[8] =
              isByteOp(byte) ? &byte_reg : &word_reg;

          const char *r_m = (*register_table)[r_m_index];
          const char *reg = (*register_table)[reg_index];
          if (isRegSource(byte)) {
            strcat(instruction, r_m);
            strcat(instruction, ", ");
            strcat(instruction, reg);
          } else {
            strcat(instruction, reg);
            strcat(instruction, ", ");
            strcat(instruction, r_m);
          }
          strcat(instruction, "\n");
        }
      } else {
        char str[4];
        std::snprintf(str, sizeof(str), "%02x\n", byte);
        strcat(instruction, str);
      }
      printf("%s", instruction);
    }

    if (bytes_read < buf_size) {
      if (ferror(f)) {
        display_error_badge();
        printf("read error\n\n");
        return 1;
      }
      if (feof(f)) {
        break;
      }
    }
  }

  fclose(f);

  return 0;
}
