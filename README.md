# RayTracer
#### Author: Michael MacFarlane

A ray tracing engine that renders scenes comprised of 3D objects and lights.

RayTracer is run via comand line with a mandatory scene file parameter. The maximum recursion depth of internal reflections in spheres 10.

This project was originally compiled using g++ and on linux. It was later converted to a VC++ project for Windows.

GLUT (or freeglut) is required to build and run RayTracer.

### Building an x86 executible using 64-bit Windows
Copy glut32.dll to "C:\Windows\SysWOW64".

Copy glut.h to "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\gl".

Copy glut32.lib to "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib".

### To run the program type:
.\RayTracer.exe <scene file>

### Example command:
.\Release\RayTracer.exe ".\Scenes\sceneA.txt"

### Sample scene descriptions:
Scenes\sceneA.txt

Scenes\sceneB.txt

Scenes\sceneC.txt

### Sample scene output:
sceneA.png

sceneB.png

sceneC.png
