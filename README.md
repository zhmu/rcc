# rcc

This is an attempt to write a C compiler, *rcc*. It is inspired by Warren Toomey's [cwj](https://github.com/DoctorWkt/acwj/)].

## Goals

- Be self-compiling, i.e. you can use GCC/Clang to compile *rcc* and the resulting binary can be used to compile *rcc* itself.
- Have decent C++ tests in an attempt to prove correctness and aid development
- Attempt to generate at least halfway optimised code

## Non-goals

- Any other target than x86_64
- C++ support
- Compilation speed is not a concern at the moment
- Diagnostics needn't be as good as GCC/Clang


## License

zlib license
