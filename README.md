# 8086 Decoder

![Status: WIP](https://img.shields.io/badge/status-WIP-orange)
![Language: C++20](https://img.shields.io/badge/language-C%2B%2B20-blue)

An 8086 disassembler written in C++.

The project is currently a work in progress and intentionally narrow in scope.

## Current Support

- `MOV` opcode family (`100010dw`)
- Register-to-register decoding (`mod = 11`)
- 8-bit and 16-bit register names (`al`..`bh`, `ax`..`di`)
- Basic file decoding flow with 16-bit assembly-style output header

Notes:

- Unsupported bytes are currently printed as raw hex.
- Memory addressing modes and displacement handling are not implemented yet.

## Usage

From the repository root:

```bash
clang++ -std=c++20 -O2 src/main.cpp -o decode8086
./decode8086 path/to/binary
```

Help:

```bash
./decode8086 --help
```

Example output starts with:

```asm
; yourfile.bin disassembly:

bits 16
```

## Roadmap

- Add remaining `MOV` forms (memory/register, immediate, accumulator variants)
- Implement `mod/reg/rm` memory decoding with 8-bit and 16-bit displacement
- Improve parser safety around truncated/invalid instruction streams
- Add automated tests with known input/output fixtures
- Expand instruction coverage beyond `MOV`

## Development

- Language: C++20
- Entry point: `src/main.cpp`
