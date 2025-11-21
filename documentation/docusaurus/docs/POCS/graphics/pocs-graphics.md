# Graphics Libraries POCs Documentation

This document explores different graphics libraries for 2D rendering in our C++ game project. We've tested three popular libraries: SFML, SDL2, and Raylib, to determine the most suitable one for our needs.

## Objective

The goal of these POCs is to evaluate graphics libraries based on:

- **Ease of use**: API simplicity and learning curve
- **Performance**: Rendering speed and resource management
- **Features**: Available functionality for 2D games
- **Cross-platform support**: Compatibility with Linux and Windows
- **Integration**: How well it fits with our C++ codebase and ECS
- **Community and documentation**: Available resources and support
- **Licensing**: Open-source and permissive licenses

## Tested Libraries

### SFML (Simple and Fast Multimedia Library)
A modern, object-oriented multimedia library for C++.

See [SFML POC](poc-sfml.md)

### SDL2 (Simple DirectMedia Layer)
A cross-platform development library designed to provide low-level access to audio, keyboard, mouse, joystick, and graphics hardware.

See [SDL2 POC](poc-sdl2.md)

### Raylib
A simple and easy-to-use library to enjoy videogames programming.

See [Raylib POC](poc-raylib.md)

## Conclusion

Based on our evaluation, we have chosen **SFML** as our graphics library for the following reasons:

### Personal Arguments
- Familiarity with the library
- Pleasant to use and simple
- Powerful and sufficiently performant
- No encapsulation and object-oriented
- Compatible with Linux & Windows

### Objective Arguments
- **Object-oriented API**: Consistent and intuitive classes (vs. C functions in SDL2/Raylib)
- **Automatic resource management**: RAII prevents memory leaks
- **Optimized 2D rendering**: Batching and vertex arrays for thousands of entities
- **Integrated advanced effects**: Built-in shader and effect support
- **Complete ecosystem**: Rendering, audio, networking, and system in one library
- **Extended cross-platform**: Android, iOS, consoles support
- **Excellent documentation**: Rich tutorials and active community
- **Modern C++ integration**: Smart pointers, exceptions
- **Multithreading-friendly**: Easy separation of rendering and logic
- **Scalable**: Suitable for medium to large projects with more "headroom" than Raylib

SFML provides the best balance of ease of use, performance, and features for our team's needs and project scope.