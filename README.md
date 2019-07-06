# detect-cpu
C code for cpu detection, arch detection for gcc/clang


The intention of this repo is to have one binary to check for the cpu flags and return the arch name,
compatible to the gcc detection. This small code is suitable for arch dependencies e.g. in containers
without the overhead of a compiler.
