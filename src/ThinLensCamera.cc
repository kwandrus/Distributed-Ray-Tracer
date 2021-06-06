
#include "ThinLensCamera.h"
#include "Ray.h"
#include "Math.h"
#include <math.h>

ThinLensCamera::ThinLensCamera(const Point& eye, const Point& lookat, const Vector& up,
                             double hfov, double aperture, int numDOFSamples)
  : eye(eye), lookat(lookat), up(up), hfov(hfov), aperture(aperture), numDOFSamples(numDOFSamples)
{
}

ThinLensCamera::~ThinLensCamera()
{
}

void ThinLensCamera::preprocess(double aspect_ratio)
{
    lookdir = lookat-eye;
    focalLength = lookdir.length();
    lookdir.normalize();
    u = Cross(lookdir, up);
    v = Cross(u, lookdir);
    double ulen = tan(hfov/2.*M_PI/180.);
    u.normalize();
    u *= ulen;
    double vlen = ulen/aspect_ratio;
    v.normalize();
    v *= vlen;
}

void ThinLensCamera::makeRay(Ray& ray, const RenderContext& context, double x, double y) const
{
    Vector rayDirection = lookdir + u * x + v * y;
    rayDirection.normalize();
    Point focalPoint = eye + focalLength * rayDirection;

    // compute vector with random values between 0.0 and 1.0
    double randNum1 = (double(rand()) / double(RAND_MAX)) - 0.5;
    double randNum2 = (double(rand()) / double(RAND_MAX)) - 0.5;
    double randNum3 = (double(rand()) / double(RAND_MAX)) - 0.5;
    Vector shiftOrigin(randNum1, randNum2, randNum3);

    // shift the origin of the ray from the camera by the aperture to produce depth of field effect
    Point newOrigin = eye + aperture * shiftOrigin;
    rayDirection = focalPoint - newOrigin;
    rayDirection.normalize();

    ray = Ray(newOrigin, rayDirection);
}
