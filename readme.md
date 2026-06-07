<p align="center">
<img src="assets/images/demo_gameplay_octree.gif" alt="OpenGL Voxel Renderer" width="100%"/>
</p>

# OpenGL Voxel Renderer

## Overview
A voxel renderer built from scratch in C++ and OpenGL. It features a custom Entity Component System (ECS), procedurally generated terrain, and an optimized rendering pipeline.

## Technical Highlights & Architecture

* **Procedural Chunk Generation:** The world is divided into chunks, each managed by a separate octree. Voxels coordinates are stored as local `uint8_t` XYZ values, which are offset by the chunk's world position during rendering and collision detection.
* **Octree System:** Chunks are spatially partitioned using an octree data structure. (Support for multiple Level of Detail (LOD) is in active development).
* **Memory Optimization:** Each voxel is tightly bit-packed into just two `uint32_t` variables, minimizing memory footprint and improving cache locality. 
* **Aggressive CPU-Side Culling:** Face visibility is precomputed on the CPU, seamlessly handling occlusion across chunk boundaries. Completely occluded geometry is stripped out before issuing draw calls.
* **Geometry Shader Expansion:** The vertex data sent to the GPU contains only an array position and a face visibility bitmask. The Geometry Shader reads this bitmask and dynamically emits only the exposed faces as optimized triangle strips.
* **Asynchronous Generation:** Procedural chunk generation and mesh building are offloaded to a background thread to ensure the main render loop remains smooth. 
* **Vox Model Loading:**  The engine includes a custom `.vox` file loader, allowing MagicaVoxel models to be imported and inserted directly into the generated world. 
* **Destructible Terrain:** Real-time terrain modification and destruction are in a late stage of development.

## Build Instructions (Windows / MSVC)

This project is configured as a native Visual Studio project. Third-party compiled binaries are excluded and must be provided locally.

### Prerequisites
You will need the following dependencies:
* [GLFW](https://www.glfw.org/download.html) (Pre-compiled binaries for Windows)
* [GLEW](https://glew.sourceforge.net/)
* [GLM](https://github.com/g-truc/glm)
* [GLAD](https://glad.dav1d.de/)

### Setup & Build
1. Clone the repository:
   ```bash
   git clone https://github.com/mathijs28498/opengl-voxels-1.git
   ```
2. Create a `libraries` folder in the root directory.
3. Place the downloaded `include` and `bin` folders from your dependencies into the `libraries` folder. The `.vcxproj` expects the following structure:
   * `libraries/include/glfw`
   * `libraries/include/glew`
   * `libraries/include/glm`
   * `libraries/include/glad`
   * `libraries/bin/glfw`
   * `libraries/bin/glew`
4. Open the Visual Studio solution/project file in **Visual Studio 2022 or newer**.
5. Set the build configuration to **Release** or **Debug** and platform to **x64**.
6. Build the solution and run.

## License
This project is licensed under the [MIT License](LICENSE).