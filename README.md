# Distributed-Ray-Tracer
Base program was written by Steven Parker at the University of Utah. My additions include implementing the recursive ray tracing algorithm and producing distributed ray tracing effects, namely anti-aliasing, depth of field, and motion blur. The core files for these effects can be found in Scene.cc, LambertianMaterial.cc, and ThinLensCamera.cc.  

## Base
The base image shows the final ray traced product of Program 1 without the effects of anti-aliasing, depth of field and motion blur. The maximum ray depth is 4.  

![base image](./final%20images/base.png)

## Anti-Aliasing
In the input file, I specify the number of samples to take per pixel labeled ‘samplesperpixel’. I found that 25 samples using jittered sampling produce a good enough effect, so the below image displays that result. The end effect shows smoothness around the edge of the spheres and the edge of the plane.  

![base image](./final%20images/anti_aliasing.png)

## Depth of Field
In the input file, I specify the parameters for a thin lens camera to showcase a depth of field effect. It keeps the same 4 parameters as the pinhole camera (eye, lookat, up, and hfov), but I add the aperture value and the number of samples to use. The focal length is computed from the length of the vector (lookat – eye). Due to the random sampling, the resulting image is quite noisy, so I increased the number of samples to 100 to substantially reduce this. The end effect shows one sphere in focus while the others are noticeably out of focus.  

![base image](./final%20images/depth_of_field.png)  

![base image](./final%20images/depth_of_field2.png)

## Motion Blur
In the input file, I add to a primitive’s parameters by specifying the motion velocity and the number of samples to use. To create the motion effect, I multiply the velocity vector by a random t value and add that to the center of the primitive, in this case the sphere. Again, due to the random sampling, the resulting image is quite noisy, so I increased the number of samples to 100 to substantially reduce this. The end effect shows one sphere seemingly moving in the image.  

![base image](./final%20images/motion_blur.png)  

![base image](./final%20images/motion_blur2.png)

## Combined Effects
Anti-aliasing, depth of field, and motion blur are all active in the following images.

### Blue sphere in focus, red sphere in motion
![base image](./final%20images/final.png)  

### White sphere in focus, blue sphere in motion
![base image](./final%20images/final2.png)
