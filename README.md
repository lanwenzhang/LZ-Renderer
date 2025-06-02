# LZ-Renderer
A lightweight framework implementing real-time rendering techniques with C++ and OpenGL

# Physically-Based Renderer

A physically-based rendering system using the Cook-Torrance BRDF model and image-based lighting

## Screenshots
* PBR

![LZ-Renderer](data/screenshot_01.gif)

* Deferred Rendering

![LZ-Renderer](data/screenshot_02.gif)


## Features
* Physically-based Rendering with GGX-Smith BRDFs
* Image-based lighting
* Gamma correction
* Multisample anti-aliasing
* Bloom and HDR tone mapping

## Build

### Windows
```
git clone --recursive https://github.com/lanwenzhang/PBR-Renderer.git
cd PBR-Renderer
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
```

## Dependencies
* [glfw](https://github.com/glfw/glfw)
* [glad](https://glad.dav1d.de/)
* [imgui](https://github.com/ocornut/imgui)
* [stb](https://github.com/nothings/stb)
* [tinyexr](https://github.com/syoyo/tinyexr)
* [assimp](https://github.com/assimp/assimp)

## Reference
* [Learn OpenGL Tutorial](https://learnopengl.com)
* [OGLDEV OpenGL Tutorial](https://ogldev.org)
* [GAMES 202, Advanced Real-time Rendering](https://sites.cs.ucsb.edu/~lingqi/teaching/games202.html)
