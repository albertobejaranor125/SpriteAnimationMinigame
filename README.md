# SpriteAnimationMinigame

A small 2D animation demo built with SDL2 and C++ using sprite sheets. This project demonstrates time-based sprite animation, an entity system, and basic animation state switching (idle, walking, etc.).

## 🚀 Features

- Sprite sheet loading and rendering
- Time-based animation using `deltaTime`
- Entity class with integrated `Animation` component
- Input-based animation switching (idle/walk)
- Written in modern C++ with SDL2
- CMake support for building across platforms

  ## 🛠️ Requirements

- C++ compiler (Visual Studio, g++, clang++)
- [CMake](https://cmake.org/)
- SDL2
- SDL2_image

## 🔧 Build Instructions

### Using CMake (recommended)

```bash
mkdir build
cd build
cmake ..
cmake --build .
