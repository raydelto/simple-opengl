# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Build System

This project uses a Makefile with platform-specific configurations for Linux, macOS, and Windows (MinGW64).

### Build Commands

**On Windows (MinGW64):**
```pwsh
make
```

**Clean build artifacts:**
```pwsh
make clean
```

**Run the application:**
```pwsh
.\bin\main.exe
```

### Platform-Specific Dependencies

- **Windows (MinGW64)**: Requires GLEW, GLFW3, OpenGL32 libraries installed in `C:\msys64\mingw64\lib`
- **Linux**: Requires libGL, libGLEW, libglfw, libdl
- **macOS**: Uses GLAD instead of GLEW, requires GLFW from Homebrew

All platforms use C++17 standard (`-std=c++17`).

## Project Architecture

### Core Components

**MD2 Model Loader (`Md2` class)**
- Loads Quake 2 MD2 model format with frame-based animation
- Uses vertex interpolation for smooth animation between keyframes
- Implements double-buffering: stores both current and next frame vertex data in GPU buffers
- Creates separate VAO/VBO pairs per animation frame for efficient rendering

**Rendering Pipeline**
1. Vertex shader (`shaders/basic.vert`) performs frame interpolation on the GPU
2. Fragment shader (`shaders/basic.frag`) applies texture mapping
3. Animation system cycles through frames with configurable interpolation speed

**OpenGL Initialization (`OpenGLHandler` class)**
- Sets up GLFW window and OpenGL context
- Handles keyboard input (SPACE key to pause/resume rotation)
- FPS counter display
- Window resize callbacks

**Shader Management (`ShaderProgram` class)**
- Loads and compiles vertex/fragment shaders from files
- Uniform location caching for performance
- Supports common uniform types (float, vec2-4, mat4)

**Texture Management (`Texture2D` class)**
- Loads TGA textures for model skins
- Optional mipmap generation

### Key Architectural Patterns

**Frame Interpolation System**
- Each vertex buffer stores 8 floats per vertex: current position (3), next position (3), texture coords (2)
- Vertex shader interpolates between frames using a time-based interpolation factor
- Eliminates need to upload new vertex data every frame

**Memory Layout**
- All MD2 frames pre-loaded into separate GPU buffers at initialization
- Frame indices map animation frames to vertex ranges in the combined buffer
- Uses `std::unique_ptr` for RAII memory management of model data and OpenGL wrapper objects

### Directory Structure

- `src/` - C++ source and headers (Md2, OpenGLHandler, ShaderProgram, Texture2D, main)
- `shaders/` - GLSL vertex and fragment shaders
- `data/` - MD2 models and TGA textures (female.md2, female.tga)
- `include/` - Third-party headers (GLM math library for matrix/vector operations)
- `bin/` - Build output directory (generated, git-ignored)

### Platform Compatibility Notes

- Uses conditional compilation for GLEW (Windows/Linux) vs GLAD (macOS)
- File I/O uses `fopen_s` on Windows (`WIN32` define) for security
- GLM library provides cross-platform math utilities for transformations
