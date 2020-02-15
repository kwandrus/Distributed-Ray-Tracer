
#ifndef ThinLensCamera_h
#define ThinLensCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"

class ThinLensCamera : public Camera {
 public:
  ThinLensCamera(const Point& eye, const Point& lookat, const Vector& up, double hfov, double aperture, int numDOFSamples);
  virtual ~ThinLensCamera();
  
  virtual void preprocess(double aspect_ratio);
  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y) const;

  virtual int getNumSamples() const {
      return numDOFSamples;
  }

 private:
  ThinLensCamera(const ThinLensCamera&);
  ThinLensCamera& operator=(const ThinLensCamera&);

  //float lensRadius;
  //float viewPlaneDist;
  double focalLength;
  double aperture; // n
  int numDOFSamples;

  Point eye;
  Point lookat; // defines focal plane
  Vector up;
  double hfov;

  Vector u;
  Vector v;
  Vector lookdir;
};

#endif

