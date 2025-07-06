# GL-Render-Pad
A lightweight framework implementing real-time rendering techniques with C++ and OpenGL

## Screenshots
* PBR

![GL-Render-Pad](data/screenshot_01.gif)


* Deferred Rendering

![GL-Render-Pad](data/screenshot_02.gif)

* PCF Soft Shadow

![GL-Render-Pad](data/screenshot_03.gif)



## Features
- Physically-based Rendering with GGX-Smith BRDFs
* Image-based lighting


- Deferred Rendering
  - Geometry pass
      - Instanced drawing
      - G buffer
        - Position attachment
        - Normal attachment
        - Albero attachment (diffuse + specular)
  
  - Lighting pass
     - Blinn-Phong model
     - Point lights

- Post processing
  - Gamma correction
  - Multisample anti-aliasing
  - Bloom and HDR tone mapping

## Build

### Windows
```
git clone --recursive https://github.com/lanwenzhang/GL-Render-Pad.git
cd GL-Render-Pad
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
* [Poly Haven](https://polyhaven.com/)
* [three.js](https://threejs-journey.com/)
