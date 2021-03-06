# Volume Rendering for Graduation Project 2021

>

## About CODE

```bash
.
├── CMakeLists.txt
├── README.md
├── build
├── include
│   ├── custom*(class abstract header)*
│   │   ├── index_buffer.h
│   │   ├── renderer.h
│   │   ├── shader.h
│   │   ├── texture.h
│   │   ├── vertex_array.h
│   │   ├── vertex_buffer.h
│   │   └── vertex_buffer_layout.h
│   ├── libs.h*(include all header we need)*
│   ├── utils.h
│   └── vendor
│       └── stb_image.h
├── resource
│   ├── shaders*(shader files)*
│   │   ├── Basic.shader
│   │   ├── HelloWorld.shader
│   │   └── RayTrace.glsl
│   └── textures
│       ├── envmap6.jpg
│       └── ttt.png
├── run.sh
└── src
    ├── custom*(class abstract implementation)*
    │   ├── index_buffer.cpp
    │   ├── renderer.cpp
    │   ├── shader.cpp
    │   ├── texture.cpp
    │   ├── vertex_array.cpp
    │   ├── vertex_buffer.cpp
    │   └── vertex_buffer_layout.cpp
    ├── main.cpp
    └── utils.cpp
```


## Objective

To implement a volume rendering algorithm and try to optimize its efficiency.

Use C++ with OpenGL & GLSL as programming language

## Schedule

### Timeline

- March 1 - March 5:
  - Thesis proposal
    - Figure out the main purpose
  - Bibliography report
    - Read papers
- April 19 - April 23:
  - Mid-term examination
- May 31 - June 4:
  - Final examination
  - Finish paper

### Roadmap

| Date | C++ | CG  | OpenGL | GLSL | Paper reading |
| ---- | --- | --- | ------ | ---- | ------------- |
|      |     |     |        |      |               |
|      |     |     |        |      |               |
|      |     |     |        |      |               |
|      |     |     |        |      |               |

## TODO

- [ ] https://alex.vlachos.com/graphics/Alex_Vlachos_Advanced_VR_Rendering_Performance_GDC2016.pdf
- [ ] https://www.youtube.com/watch?v=ya8vKZRBXdw
- [ ] OpenGL GLSL
- [ ] github: volume rendering glsl
- [ ] Data source?

### Paper reading

#### [Optical models for direct volume rendering](https://ieeexplore.ieee.org/abstract/document/468400)



#### [Direct Volume Rendering: A 3D Plotting Technique for Scientific Data](https://ieeexplore.ieee.org/document/4418774)





#### Local Ambient Occlusion in Direct Volume Rendering

TODO

#### REAL-TIME VOLUME RENDERING INTERACTION IN VIRTUAL REALITY

TODO

#### A Virtual Reality Visualization Tool for Neuron Tracing

- [x] http://sci.utah.edu/~will/papers/vrnt/vr-neuron-tracing.pdf(downloaded)

Author: https://www.willusher.io/

Reading draft locates in [notes](https://github.com/CoyoteWaltz/MarkdownNotes/blob/master/02learning_notes/computer_graphics/paper_reading.md)

### VR intro

_HMD_: Head mounted display, the fancy glasses wore on the user's head

### Basic knowledge of CG

#### Learning materials

- Keep learning a course from [Games101](https://www.bilibili.com/video/BV1X7411F744) and take some [notes](https://github.com/CoyoteWaltz/MarkdownNotes/tree/master/02learning_notes/computer_graphics)
- Books?

### Go deep in C++

- Figure out what is CMake and makefile.
- Third-part vector calculation library for C++: [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

#### OpenGL

#### OpenCV

#### How the data interact with GPU

### GLSL

> **OpenGL Shading Language** (**GLSL**) is a high-level shading language with a syntax based on the C programming language. It was created by the OpenGL ARB (OpenGL Architecture Review Board) **to give developers more direct control of the graphics pipeline** without having to use ARB assembly language or hardware-specific languages. -- from [wiki](https://en.wikipedia.org/wiki/OpenGL_Shading_Language)

_My perspective: One of the programmable interfaces provided for developer to control the process of shading while other parts is hardcode in GPU render pipeline._

#### History

Originally introduced as an extension to OpenGL 1.4, GLSL was formally included into the OpenGL 2.0 core in 2004 by the OpenGL ARB. It was the first major revision to OpenGL since the creation of OpenGL 1.0 in 1992.

- Cross-platform
- Write shaders
- Each hardware vendor includes the GLSL compiler in their drive

#### Shader

Ended with a **main** function which returns a color assigned to the global variable **gl_FragColor**.

Precision: is crucial to type float. Lower precision means faster rendering, but at the cost of quality. Medium precision => `mediump`, `lowp`, `highp`

Uniforms: You can picture the uniforms like little bridges between the CPU and the GPU.

Varying: **gl_FragCoord**: a default input vec4, which holds the screen coordinates of the *pixel* or *screen fragment* that the active thread is working on. It varies from thread to thread.



*You will discover that sometimes coding in GLSL is very similar to putting ships inside bottles. Is equally hard, beautiful and gratifying.*



### 3D rendering, volume rendering

> 3D-rendering is a kind of technology for processing 3D models into 2D images on the computer.
>
> Rendering methods:
>
> - [scanline rendering](https://en.wikipedia.org/wiki/Scanline_rendering):
> - [ray tracing](<https://en.wikipedia.org/wiki/Ray_tracing_(graphics)>):
> - [radiosity](<https://en.wikipedia.org/wiki/Radiosity_(computer_graphics)>):
> - ...
> - Volume rendering: 3D scalar field
>
> -- from [wikipedia](https://en.wikipedia.org/wiki/3D_rendering)

#### Terminologies

- _voxel_: volume element, similar to the terms _pixel_ for "picture element"
- _texel_: texture element(TExture + ELement)
- _reconstruction_: As the voxel is the data correspoding to the position in the column, the intermediate point is obtained by **interpolating** data at neighboring volume elements. This process is very important in volume rendering and processing applications.
- _optical model_: describes how particles in the volume interact with light. Most commonly assuming that the particles emit and absorb light simultaneously. More complex models incorporate local illumination and volumetric shadows, and they account for light scattering effects.
- _transfer function_: emphasizes and classify the features of the interest in the data. Typically, transfer functions are implemented by _texture **lookup** tables_, though simple functions can also be computed in the fragment shader.
- _emission-absorption model_: More vividly, the color part RGB is the emission or reflection of the light while the alpha channel approximates the absorption.
- _texture-based volume rendering_: performs the **sampling and compositing** steps by rendering a set of 2D geometric primitives inside the volume.
- _proxy geometry_: is **rasterized and blended** into the frame buffer in back-to-front or front-to-back order. It's -- I think -- the real object which will be drawn on the screen.
- _isosurface_: “It is a surface that represents points of a constant value (e.g. pressure, temperature, velocity, density) within a volume of space; in other words, **it is a level set of a continuous function whose domain is 3D-space**.” -- from [wikipedia](https://en.wikipedia.org/wiki/Isosurface). Just like the contour line which is projected into 2D-space. So, in 3D-space an isosurface represents the same property of a bundle of points.

#### Volume rendering

> Volume rendering is **a set of techniques** used to display a 2D projection of a 3D discretely sampled data set, typically a 3D [scalar field](https://en.wikipedia.org/wiki/Scalar_field).
>
> - Scalar field: each point in the space is a **scalar**
> - A typical 3D data set: is a group of 2D slice images acquired by a [CT](https://en.wikipedia.org/wiki/Computed_axial_tomography), [MRI](https://en.wikipedia.org/wiki/Magnetic_resonance_imaging), or [MicroCT](https://en.wikipedia.org/wiki/Microtomography) [scanner](https://en.wikipedia.org/wiki/Image_scanner).
>
> -- from [wikipedia](https://en.wikipedia.org/wiki/Volume_rendering)

For instance, a series of 2D **slice** images of a human brain can be assembled to render 3D volume rendered images using a volume rendering algorithm.

Once that 3D data set is captured, rendering a 2D projection of it is next. However, it is first necessary to define the volume relative to the position of the camera in space. Users then must define each voxel's color and opacity, typically using an RGBA transfer function.

#### Why?

Volume rendering allows users to visualize three-dimensional scalar fields. This is important for any industry that produces 3D data sets for analysis, including physics, _medicine_, disaster preparedness, and more.

Volume rendering is important to understand the data from CAT, MRI scanners, complex fluid dynamics, data from seismic events, and other volumetric data for which geometric surfaces are unavailable or just too difficult or cost-prohibitive to generate.

#### Common Volume Rendering Techniques

3D volume rendering methods can be grouped into four categories: ray casting or raymarching, resampling or shear-warp, texture slicing, and splatting.

Reference:

1. https://www.omnisci.com/technical-glossary/volume-rendering
2.