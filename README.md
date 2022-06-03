# Ray Tracer

Updated by Ilene Lu, Apr. 2019.

## Overview

This project implements a ray tracer with support of various artifact.

## Objective

1. Primitive Support
 - Plane, cube, sphere, cylinder, torus
2. Texture Mapping
 - Support both spherical mapping and cube mapping
3. Bump Mapping
4. Reflection, Refraction, Fresnel and Glossy Reflection
5. Perlin Noise
6. Motion Blur
7. Soft Shadow
8. Non-adaptive Anti-Aliasing

## Gallery

![](/results/torus.webp)

Torus

![](/results/texture_mapping.webp)

Texture Mapping

![](/results/bump_mapping.webp)

Bump Mapping

![](/results/glossy.webp)

Glossy Reflection & Refraction

![](/results/perlin.webp)

Marble Pattern generated with Perlin Noise

![](/results/motion_blur.webp)

Motion Blur (constant acceleration)

## Reference

[1] James Blinn. “Simulation of Wrinkled Surface”. In: ACM SIGGRAPH Computer Graphics 12.3 (Aug. 1978), pp. 286–292.
[2] Kenneth R. Sloan Eric A. Bier. “Two-Part Texture Mappings”. In: IEEE Computer Graphics and Applications 6.9 (Sept. 1986), pp. 40–53.
[3] John. F. Hughes. “Textures and Texture Mapping”. In: Computer graphics: principles and practice. NJ: Addison-Wesley, 2014, pp. 547–557.
[4] Ken Perlin. “An Image Synthesizer”. In: Computer Graphics 24.3 (1985).
[5] Ken Perlin. “Improving Noise”. In: ACM Transactions on Graphics 21.3 (2002), pp. 681–682.
[6] Kevin Suffern. “Glossy Reflection”. In: Ray Tracing from the Ground Up. Wellesley, Mass.: A K Peters, 2007, pp. 529–542.
