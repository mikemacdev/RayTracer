# RayTracer

A ray tracing engine that renders scenes comprised of 3D objects and lights.

RayTracer is run via comand line with a mandatory scene file parameter. The maximum recursion depth of internal reflections in spheres 10.

This project was originally compiled using g++ and on linux. It was later converted to a VC++ project for Windows.

GLUT (or freeglut) is required to build and run RayTracer.

### Install GLUT

Example locations for Visual Studio 2017

File | Location
--- | ---
glut.h | C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\include\gl
glut32.lib | C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\lib\x64
glut32.dll | C:\Windows\SysWOW64

### Run

RayTracer.exe <scene file>

Example command

RayTracer.exe .\Scenes\sceneA.txt

### Input

Scene file examples are in the Scene directory.

### Output

Scene example output images are in the Screenshots directory.
