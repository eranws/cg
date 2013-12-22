eranws 300489960
ilansh 302514401

Grading
---
Please check and grade ex2 as well.


Implementation:
---
In ex2, we implemented the trasnform by rotating and translating the object, and scale was implemented by changing FoV.
The circle and the object are in two different buffers.

In ex3, we imeplemented the phong lighting model for both phong shading - in the fragment shader, and for gouroaud shading - in the vertex shader.
The normals and vertices for basic normal mode are buffered without ebo (duplicate vertices).
The normals and vertices for advanced normal mode are buffered using an ebo.

Files sent:
---
Readme.txt - this file

shaders/CircleShader.frag
shaders/CircleShader.vert
shaders/GuShader.frag
shaders/GuShader.vert
shaders/PhongShader.frag
shaders/PhongShader.vert
shaders/RgbShader.frag
shaders/RgbShader.vert
ShaderIO.cpp

Model.cpp
Model.h
ex3.cpp
ex3.h
ShaderIO.h
ShaderIO.cpp

Makefile


website used:
---
http://open.gl/
openmesh.org

