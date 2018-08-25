# GLUT_RAY_TRACER
C++ Ray tracing app

![](https://github.com/robloomes/GLUT_RAY_TRACER/blob/master/finalnoaa.png)

Build instructions:
-------------------
g++ -o “%e” Plane.cpp Ray.cpp RayTracer.cpp SceneObject.cpp Sphere.cpp
Tetrahedron.cpp TextureBMP.cpp -lGL -lGLU -lglut

Run times
-------
If you limit the window to a 500x500 pixel size, then the image usually renders in
around 20 seconds. Rendering full screen can be upwards of 1 minute.
