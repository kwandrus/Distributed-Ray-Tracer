
#ifndef LambertianMaterial_h
#define LambertianMaterial_h

#include "Material.h"
#include "Color.h"

class LambertianMaterial : public Material {
 public:
  LambertianMaterial(const Color& color, float Kd, float Ka, float Ks, int n);
  virtual ~LambertianMaterial();

  virtual Color shade(const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;
  virtual bool isReflective() const;
  virtual Color getColor() const;
  virtual float getKs() const;

 private:
  LambertianMaterial(const LambertianMaterial&);
  LambertianMaterial& operator=(const LambertianMaterial&);

  Color color;
  float Kd;
  float Ka;
  float Ks;
  int n;
};

#endif
