
#include "Sphere.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <math.h>

Sphere::Sphere(Material* material, const Point& center, double radius, Vector motionvelocity, int motionSamples)
  : Primitive(material), center(center), radius(radius)
{
    motionVelocity = motionvelocity;
    numMotionSamples = motionSamples;
    if (motionVelocity != 0) {
        isMoving = true;
    }
    inv_radius = 1. / radius;
}

Sphere::~Sphere()
{
}

void Sphere::getBounds(BoundingBox& bbox) const
{
    Vector diag(radius, radius, radius);
    bbox.extend(center + diag);
    bbox.extend(center - diag);
}

void Sphere::intersect(HitRecord& hit, const RenderContext&, const Ray& ray) const
{
    Vector O(ray.origin() - center);
    const Vector& V(ray.direction());
    double b = Dot(O, V);
    double c = Dot(O, O) - radius * radius;
    double disc = b * b - c;

    if(disc > 0){
        double sdisc = sqrt(disc);
        double root1 = (-b - sdisc);

        if(!hit.hit(root1, this, matl)){
            double root2 = (-b + sdisc);
            hit.hit(root2, this, matl);
        }
    }
}

void Sphere::move(double time) {
    Vector jitteredVec = motionVelocity * time;
    center += jitteredVec;
}

void Sphere::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const
{
    normal = (hitpos - center) * inv_radius;
}
