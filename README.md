# Blenter

Blenter is a work-in-progress game engine. It is named using Dwight's naming convention for existing-adjacent people (e.g. a dentist named Crentist).

### Build

*Requirements: OpenGL 3.3+*

1. `vcpkg install`
2. `mkdir build && cd build`
3. `cmake .. -DCMAKE_TOOLCHAIN_FILE <path-to-vcpkg.cmake>`
4. `make`