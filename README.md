Compilation:
Submitted version is complied and run on MacOSX without running on a lab machine;

Manual:

Extra Features: supersampling using 9 rays per pixel;

screenshot.png contains the render of simple-cows.lua with supersampling and resolution 1024 * 1024

Note that the required images nonhier.png, macho-cows.png and simple-cows.png in Assets folder are rendered without supersampling.

In order to render the bounding volume, switch the flag drawBB in Mesh.cpp to true, to turn off, set the flag to false;

The unique scene contains the base node Scene which are rotated along X axis, and everything except the green cube are children of base node each with their own transformation. The green cube however, is the child of the orange box, and the cube has its own local translation and rotation. The scene also contains two light source, the magenta one from left of the scene and a grey one from right of the scene. 
