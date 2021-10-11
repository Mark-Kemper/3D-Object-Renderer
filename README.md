# 3D-Object-Renderer
This project allows for the rendering of both rounded and flat surfaces in different colors, positions, and sizes. The camera's focal length may also be modified, which allows for the perspective effect to be exaggerated or diminished. It works by creating a vector with an origin at a light emitter and its direction randomized. It then calculates the first collision of that light, and the bounce of the light off the surface determines which pixel on the screen is modified and the intensity of the brightness it is given. This process is repeated until the render is complete.

Time-lapse of a render:

https://user-images.githubusercontent.com/91701863/136714874-a154a114-b5ac-4f96-aacf-f06a314000e1.mp4
