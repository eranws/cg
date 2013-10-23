eranws 300489960

Implementation:
---
To draw a circle we approximated it as a polygon with many edges (~100). first we tried to use GL_POLYGON but for some reason it didn't work, so we used GL_TRIAGLE_FAN.
(some forums say that GL_POLYGOM is fairly slow anyway)

for the checkerboard pattern we output the position of the vertex to the fragment shader, and then tested it (some algebra and xor) if to paint it with color or with black.


website used:
http://open.gl/drawing
http://www.opengl.org/sdk/docs/man/xhtml/glPolygonMode.xml
http://www.opengl.org/discussion_boards/showthread.php/169290-Drawing-speed-GL_TRIANGLES-vs-GL_POLYGON