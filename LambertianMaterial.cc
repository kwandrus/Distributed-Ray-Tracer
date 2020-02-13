
#include "LambertianMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
using namespace std;

LambertianMaterial::LambertianMaterial(const Color& color, float Kd, float Ka, float Ks, int n)
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks), n(n)
{
}

LambertianMaterial::~LambertianMaterial()
{
}

bool LambertianMaterial::isReflective() const {
    if (Ks > 0)
        return true;
    return false;
}

Color LambertianMaterial::getColor() const {
    return color;
}

float LambertianMaterial::getKs() const {
    return Ks;
}

Color LambertianMaterial::shade(const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color&, int) const
{
  const Scene* scene = context.getScene();
  const vector<Light*>& lights = scene->getLights();
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  Vector normal;
  hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
  normal.normalize();
  double costheta = Dot(normal, ray.direction());
  if(costheta > 0)
    normal = -normal;

  const Object* world = scene->getObject();

  // swap these lines to toggle global illumination
  //Color light = scene->getAmbient()*Ka;
  Color light(0, 0, 0);

#if 0
  for(vector<Light*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++){
#else
    Light*const* begin = &lights[0];
    Light*const* end = &lights[0]+lights.size();
    while(begin != end){
#endif
    Color light_color;
    Vector light_direction;
    double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
    light_direction.normalize();
    double cosphi = Dot(normal, light_direction);

    Vector reflect_direction = 2 * Dot(normal, light_direction) * normal - light_direction;
    reflect_direction.normalize();
    double cosbet = Dot(reflect_direction, ray.direction());
    if(cosphi > 0){
      // Cast shadow rays...
      HitRecord shadowhit(dist);
      Ray shadowray(hitpos, light_direction);
      world->intersect(shadowhit, context, shadowray);
      if(!shadowhit.getPrimitive())
        // No shadows...
        light += light_color*(Kd*cosphi + Ks * pow(cosbet, n));
    }
  }
  return light*color;
} 
