
#ifndef Object_h
#define Object_h

#include "Vector.h"

class BoundingBox;
class Color;
class HitRecord;
class Ray;
class RenderContext;

class Object {
 public:
  Object();
  virtual ~Object();

  virtual void preprocess();
  virtual void getBounds(BoundingBox& bbox) const = 0;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;

  void setMotion() {
      isMoving = true;
  }
  bool getMotion() const {
      return isMoving;
  }
  int getMotionSamples() const {
      return numMotionSamples;
  }
  virtual void moveObjects(double time) = 0;
  virtual void move(double time) = 0;

protected:
    bool isMoving;
    Vector motionVelocity;
    int numMotionSamples;

 private:
  Object(const Object&);
  Object& operator=(const Object&);
};

#endif

