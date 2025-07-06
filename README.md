# ZenithEngine

## 1. Introduction
ZenithEngine is a minimalist 3D rendering engine written in C++ using OpenGL 3.3. It allows loading and displaying 3D models in glTF format, with shader management, textures, cameras, and basic lighting. The project is designed to be educational, modular, and easily extensible, perfect for learning or as a foundation for more advanced graphics projects.

## 2. Installation, Compilation and Testing

### Prerequisites
- CMake >= 3.20
- A C++ compiler compatible with C++20
- [vcpkg](https://github.com/microsoft/vcpkg) for dependency management
- OpenGL 3.3+

### Main Dependencies (managed by vcpkg)
- glfw3
- glad
- nlohmann_json

### Installing Dependencies
In the project root directory:
```sh
vcpkg install glfw3 glad nlohmann-json
```

### Compilation
1. Clone the repository and open a terminal in the `ZenithEngine` folder.
2. Generate build files with CMake:
   ```sh
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
   ```
3. Compile:
   ```sh
   cmake --build build
   ```
4. The executable will be generated in `build/` or according to your IDE configuration.

### Running
Place your glTF models (e.g., `scene.gltf` and its resources) in `ZenithEngine/ZenithEngine/Models/` then run the executable:
```sh
./build/ZenithEngine
```

### Testing
No automated tests are provided at the moment. The main test consists of launching the application and verifying the 3D model display.

## 3. Code Architecture Overview

The engine core is organized around several main classes:

- **Model**: Manages loading, parsing (via nlohmann_json), and rendering of glTF models. It traverses the node hierarchy and instantiates associated Meshes.
- **Mesh**: Represents 3D geometry, encapsulates OpenGL buffers (VAO, VBO, EBO) and textures.
- **Shader**: Manages compilation, activation, and deletion of OpenGL shader programs (vertex/fragment).
- **Camera**: Handles view, projection, and user controls (movement, orientation).
- **Texture**: Loads and manages textures associated with models.

The main program entry point (`ZenithEngine.cpp`):
- Initializes GLFW/GLAD and the OpenGL window
- Loads a glTF model
- Configures camera and lighting
- Rendering loop: input handling, camera updates, model drawing

Simplified directory structure:
```
ZenithEngine/
  ├── ZenithEngine.cpp         # Main entry point
  ├── Model.h/.cpp            # glTF model management
  ├── Mesh.h/.cpp             # Mesh management
  ├── shaderClass.h/.cpp      # Shader management
  ├── Camera.h/.cpp           # Camera and controls
  ├── Texture.h/.cpp          # Textures
  ├── Models/                 # 3D glTF models
  └── ...
```

## 4. Mini User & Developer Guide

### User
- Place your glTF models in `ZenithEngine/ZenithEngine/Models/`.
- By default, the loaded file is `Models/statue/scene.gltf` (modifiable in `ZenithEngine.cpp`).
- Camera controls: use mouse and keyboard (see implementation in `Camera.cpp`).
- The application displays FPS in the window title.

### Developer
- To add a new model, modify the line:
  ```cpp
  Model model("Models/statue/scene.gltf");
  ```
- To add features (new shaders, effects, controls), extend existing classes or add new ones.
- Shaders are in `.vert` and `.frag` files (e.g., `default.vert`, `default.frag`).
- Dependencies are managed via vcpkg and CMake.

### Known Limitations
- Only glTF format is supported for models.
- No advanced animation or complex material management.
- No integrated unit tests.

---

For any contribution or question, open an issue or pull request on the GitHub repository. 