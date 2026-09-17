# WindowsWindow3D
3D Graphics from Scratch in the Windows Window API

This project was a means for me to keep up with my C++ skills while those weren't required for my Master's and PhD.
The objective was to create rudimentary 3D graphics "from scratch" starting from the windows API and by drawing triangles.
I wanted to try coding in a way that mimics how algebra works, something allowed by typed operator overloading.
I struggled with rotation for a while as I did not want to write matrices.
Thankfully, Clifford (Geometric) Algebra offers a neat way to conceptualize rotation.

To compile, you need a windows machine and the relevant lgdi/lgdi+ libraries.
The compile.bat file contains the compile command.

The executable offers a small glimpse into a 3D world. Arrow keys can be used to rotate.
![alt text](https://github.com/mathieu-pc/WindowsWindow3D/blob/main/image.png?raw=true)
