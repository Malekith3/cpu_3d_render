# CPU 3D Renderer – Pikuma Course (C++23)

This project is based on the excellent [**Pikuma 3D Graphics Programming**](https://pikuma.com/courses/learn-3d-computer-graphics-programming) course.  
It implements a full **3D graphics pipeline from scratch** on the **CPU**, with no reliance on GPU acceleration.  
All rendering is done manually, step by step, to gain a deep understanding of how real-time 3D rendering works under the hood.

---

## Goals

- Learn how a 3D rendering pipeline works by implementing it manually.
- Learn graphics fundamentals in depth, such as transformation matrices, clipping, rasterization, depth buffering, lighting, and texturing
- Gain experience with **modern C++ (C++23)** while porting from the original ANSI C course.
- Keep the project cross-platform for Windows and Linux to also become familiar with the pain this decision brings.

---

##  Libraries

- **C++23** – Modern C++ to explore newer language features.
- **SDL2** – Used only for window creation and framebuffer swapping and event handling.
- **GLM** – Math library for vector/matrix operations.

> The project is fully self-contained. You only need CMake and a compiler that supports C++23.  
> SDL2 and GLM are downloaded automatically during the build process.

---

## Current Status
> For now, I have finished this project. Most basic features are implemented.
> I am keeping a list of features that I want to implement in the future.
>
**Features**
- [x] Basic 3D scene rendering
- [x] Rasterization
- [x] Basic lighting
- [x] Basic texturing
- [x] Frustum clipping
- [x] Depth buffer (aka Z-buffer)
- [x] Basic camera
- [x] OBJ loading
- [ ] Support for multiple meshes
- [ ] Move clipping to clip space
- [ ] Optimize performance
- [ ] Index buffer for meshes

---

##  How to Build

All you need is **CMake ≥ 3.15** and a GNU compiler that supports **C++23** GCC 13-15 will work fine.

> ⚠️ **Disclaimer (Windows Edition)**
>
> Building on Windows? Good luck.  
> Tested with MinGW-w64 and MSYS2 using Ninja — it works, but expect the usual cross-platform pain.  
> I’ve embedded some MinGW runtime libs statically when building for Windows to dodge the worst of it.  
> Use `g++` from MinGW-w64 or MSYS2 (`mingw-w64-x86_64-gcc`). Pray accordingly.

### Steps to Build:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Malekith3/3d_graphics_sdl.git
   ```
2. **Build**
   ### Windows
    ````bash
    cmake -B build -G "MinGW Makefiles"
     cmake --build build -j 10
    ````
   ````bash
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
   cmake --build build -j 10
    ````
   ### Linux
    ````bash
   cmake -B build
   cmake --build build -j 10
    ````
      ````bash
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
   cmake --build build -j 10
    ````
3. **Run**
   ````bash
   #Linux
    ./build/MinimalSDL2App
   #Windows
   build\MinimalSDL2App.exe
    ````
   

> On the first run, the following dependencies will be cloned automatically:
> * SDL2 – for window/context and event handling
> * GLM – for math (matrices, vectors, transforms)

---
## Notes
This goes without saying, but this is a learning project — not something intended for production use.
Feel free to explore the code, fork the repository, or reach out if you have any questions or suggestions!
