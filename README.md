# OpenGL MD2 Loader

A modern C++17 OpenGL Quake 2 (MD2) model loader with animation support, built with GLFW and GLEW.

## Features

- MD2 model loading with frame interpolation
- Custom TGA texture loader
- GPU-based vertex interpolation for smooth animations
- Modern C++ with RAII and `std::vector`
- Clean separation of concerns (rendering, I/O, OpenGL management)

## Building on Windows with MSYS2/MinGW64

### Prerequisites

#### 1. Install MSYS2

Download and install MSYS2 from [https://www.msys2.org/](https://www.msys2.org/)

Default installation path: `C:\msys64`

#### 2. Update MSYS2

Open **MSYS2 MSYS** terminal and update the package database:

```bash
pacman -Syu
```

Close and reopen the terminal, then run again:

```bash
pacman -Su
```

#### 3. Install Required Libraries

Open **MSYS2 MinGW64** terminal (not MSYS2 MSYS!) and install the development tools and libraries:

```bash
# Install MinGW64 GCC toolchain
pacman -S mingw-w64-x86_64-gcc

# Install Make
pacman -S mingw-w64-x86_64-make

# Install OpenGL libraries
pacman -S mingw-w64-x86_64-glew
pacman -S mingw-w64-x86_64-glfw
```

Or install all at once:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-glew mingw-w64-x86_64-glfw
```

**Note:** GLM (OpenGL Mathematics library) is included in the `include/` directory and does not need to be installed separately.

#### 4. Add MinGW64 to PATH

Add `C:\msys64\mingw64\bin` to your Windows PATH environment variable, or always use the **MSYS2 MinGW64** terminal.

### Building the Project

1. Clone the repository:
```bash
git clone https://github.com/raydelto/simple-opengl.git
cd simple-opengl
```

2. Build using Make:
```bash
make
```

3. Clean build artifacts:
```bash
make clean
```

### Running the Application

From MSYS2 MinGW64 terminal:
```bash
./bin/main.exe
```

Or from Windows Command Prompt/PowerShell:
```powershell
.\bin\main.exe
```

## Usage

When the application launches, an animated MD2 model (female character) will render with smooth frame interpolation.

**Controls:**
- **SPACE**: Pause/resume rotation
- **F1**: Toggle wireframe mode
- **ESC**: Exit application

## Project Structure

```
simple-opengl/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── Md2.cpp/h             # MD2 model loader and renderer
│   ├── OpenGLHandler.cpp/h   # OpenGL/GLFW initialization
│   ├── ShaderProgram.cpp/h   # GLSL shader management
│   ├── Texture2D.cpp/h       # Texture loading
│   └── TgaLoader.cpp/h       # TGA image format loader
├── shaders/
│   ├── basic.vert            # Vertex shader with interpolation
│   └── basic.frag            # Fragment shader
├── data/
│   ├── female.md2            # MD2 model file
│   └── female.tga            # Texture file
├── include/                  # Third-party headers (GLM, GLEW, GLFW)
├── bin/                      # Build output (generated)
└── Makefile                  # Build configuration
```

## Technical Details

- **Language**: C++17
- **Graphics API**: OpenGL 3.3 Core Profile
- **Libraries**: 
  - GLEW (OpenGL Extension Wrangler)
  - GLFW3 (Window and input management)
  - GLM (Bundled - OpenGL Mathematics)
- **Build System**: GNU Make
- **Compiler**: MinGW64 GCC

## Troubleshooting

### "Cannot find -lglew32" or similar errors

Make sure you installed the libraries in the **MinGW64** environment:
```bash
pacman -S mingw-w64-x86_64-glew mingw-w64-x86_64-glfw
```

### "command not found: make"

Either:
1. Use `mingw32-make` instead of `make`, or
2. Install make: `pacman -S mingw-w64-x86_64-make`

### Application doesn't run

Ensure the required DLLs are in your PATH:
- `glew32.dll`
- `glfw3.dll`

Or run from MSYS2 MinGW64 terminal which has the correct PATH set.

## Author

Raydelto Hernandez (raydelto@yahoo.com)

## Screenshot

[![screenshot](http://www.raydelto.org/img/MD2_Loader_glfw.png)](https://github.com/raydelto/simple-opengl/)

