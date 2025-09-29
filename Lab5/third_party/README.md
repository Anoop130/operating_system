# Third Party Libraries

This directory contains third-party libraries that are included as part of the source code.

## nlohmann/json

- **Version**: 3.11.2
- **License**: MIT (see LICENSE.MIT file)
- **Source**: https://github.com/nlohmann/json
- **Description**: JSON for Modern C++ - a header-only JSON library

The library is included as a single header file (`nlohmann/json.hpp`) for easy integration and to avoid external dependencies during build.

### Usage

Include the header in your C++ files:
```cpp
#include <nlohmann/json.hpp>
```

The library is automatically available through the CMake configuration.