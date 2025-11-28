---
sidebar_position: 1
---

# POC Nlohmann JSON

This Proof of Concept demonstrates the integration and usage of the nlohmann/json library for JSON handling in C++ within the R-Type project.

## Overview

The nlohmann/json library is a popular, header-only JSON library for modern C++. It provides an intuitive syntax for parsing, serializing, and manipulating JSON data.

## Features Demonstrated

- JSON parsing from strings
- JSON serialization to strings
- Accessing JSON values
- Modifying JSON objects
- Error handling for invalid JSON

## Implementation

The POC includes:

- `CMakeLists.txt`: Build configuration using CMake and FetchContent to download the nlohmann/json library
- `main.cpp`: Example code showcasing various JSON operations
- `README.md`: Instructions for building and running the POC

## Usage Example

```cpp
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    // Create a JSON object
    nlohmann::json j = {
        {"name", "R-Type"},
        {"version", "1.0"},
        {"players", {{"player1", "Alice"}, {"player2", "Bob"}}}
    };

    // Serialize to string
    std::string s = j.dump(4); // Pretty print with 4 spaces
    std::cout << s << std::endl;

    // Parse from string
    std::string json_str = R"(
    {
        "game": "R-Type",
        "score": 1000
    }
    )";

    auto parsed = nlohmann::json::parse(json_str);
    std::cout << "Game: " << parsed["game"] << std::endl;
    std::cout << "Score: " << parsed["score"] << std::endl;

    return 0;
}
```

## Benefits

- Easy to use API
- Header-only library (no separate compilation)
- Modern C++ features
- Comprehensive documentation
- Active maintenance

## Integration in R-Type

This POC explores how JSON can be used in the R-Type project for:

- Configuration files
- Network message serialization
- Save game data
- Asset metadata

## Conclusion

The nlohmann/json library provides a robust and user-friendly solution for JSON handling in C++. Its header-only nature makes it easy to integrate into existing projects without complex build configurations.
