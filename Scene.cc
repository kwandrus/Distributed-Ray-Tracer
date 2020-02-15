 
#include "Scene.h"
#include "Background.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Primitive.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

Scene::Scene()
{
    object = 0;
    background = 0;
    camera = 0;
    ambient = Color(0, 0, 0);
    image = 0;
    minAttenuation = 0;
    numAASamples = 0;
}

Scene::~Scene()
{
    delete object;
    delete background;
    delete camera;
    delete image;
    for(int i = 0; i < static_cast<int>(lights.size()); i++){
        Light* light = lights[i];
        delete light;
    }
}

void Scene::preprocess()
{
    background->preprocess();
    for(int i = 0; i < static_cast<int>(lights.size()); i++){
        Light* light = lights[i];
        light->preprocess();
    }

    double aspect_ratio = image->aspect_ratio();
    camera->preprocess(aspect_ratio);
    object->preprocess();
}

int max(int n1, int n2) 
{
    if (n1 > n2)
        return n1;
    return n2;
}

void Scene::moveObjects(Color& result, const RenderContext& context, const Ray& ray, const Color& atten) 
{
    // if true, then one of the objects in the group is moving
    if (object->getMotion()) {
        double time = double(rand()) / double(RAND_MAX);
        object->moveObjects(time);
        result += traceRay(context, ray, atten, 0);
        object->moveObjects(-1 * time);
    }
    else {
        result += traceRay(context, ray, atten, 0);
    }
}

void Scene::render()
{
    if(!object || !background || !camera || !image){
    cerr << "Incomplete scene, cannot render!\n";
    exit(1);
    }
    int xres = image->getXresolution();
    int yres = image->getYresolution();
    RenderContext context(this);
    double dx = 2. / xres;
    double xmin = -1. + dx / 2.;
    double dy = 2. / yres;
    double ymin = -1. + dy / 2.;
    Color atten(1, 1, 1);

    for (int i = 0; i < yres; i++) {
        for (int j = 0; j < xres; j++) {
            Ray ray;
            Color result(0, 0, 0);

            // anti-aliasing and potentially depth of field (depending on camera) and motion
            // uses number of samples for anti aliasing for all components (DOF, motion)
            if (numAASamples > 0) {
                // anti-aliasing - jittered sampling
                // divide pixel into dim x dim grid and sample each cell in the grid
                int dim = (int)sqrt((float)numAASamples);
                for (int k = 0; k < dim; k++) {
                    for (int l = 0; l < dim; l++) {
                        // get x and y values within grid cell to sample
                        double randNum = double(rand()) / double(RAND_MAX);
                        double sampleX = double(j) - 0.5 + (double(l) + randNum) / double(dim);
                        randNum = double(rand()) / double(RAND_MAX);
                        double sampleY = double(i) - 0.5 + (double(k) + randNum) / double(dim);
                        double x = xmin + sampleX * dx;
                        double y = ymin + sampleY * dy;

                        camera->makeRay(ray, context, x, y);

                        // move objects in scene (if any)
                        moveObjects(result, context, ray, atten);
                    }
                }

                result /= float(numAASamples);
            }
            // depth of field and motion(no anti aliasing)
            // uses number of samples for DOF for motion too (if an object in the scene is moving)
            else if (camera->getNumSamples() > 0 || object->getMotion()) {
                int numSamples = max(camera->getNumSamples(), object->getMotionSamples());

                for (int k = 0; k < numSamples; k++) {
                    double x = xmin + j * dx;
                    double y = ymin + i * dy;

                    camera->makeRay(ray, context, x, y);

                    // move objects in scene (if any)
                    moveObjects(result, context, ray, atten);
                }

                result /= float(numSamples);
            }
            // normal ray tracing
            else {
                double x = xmin + j * dx;
                double y = ymin + i * dy;
                camera->makeRay(ray, context, x, y);
                result += traceRay(context, ray, atten, 0);
            }

            image->set(j, i, result);
        }
    }
}

Color Scene::traceRay(const RenderContext& context, const Ray& ray, const Color& atten, int depth) const
{
    Color result(0, 0, 0);
    HitRecord hit(DBL_MAX);
    object->intersect(hit, context, ray);
    if (hit.getPrimitive()) {
        // Ray hit something...
        const Material* matl = hit.getMaterial();

        result += matl->shade(context, ray, hit, atten, depth);

        if (matl->getReflective() && depth < maxRayDepth) {
            // compute intersection info
            Point hitpos = ray.origin() + ray.direction() * hit.minT();
            Vector normal;
            hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
            normal.normalize();

            // compute reflection ray info
            Vector reflect_direction = ray.direction() - 2 * Dot(normal, ray.direction()) * normal;
            reflect_direction.normalize();
            Ray reflectRay = Ray(hitpos, reflect_direction);

            // recurse reflection ray
            result += traceRay(context, reflectRay, atten, depth + 1) * matl->getKs();
        }        
    }
    else {
        background->getBackgroundColor(result, context, ray);
    }

    return result;
}

//double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
//{
//  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
//    result = Color(0, 0, 0);
//    return 0;
//  } else {
//    HitRecord hit(DBL_MAX);
//    obj->intersect(hit, context, ray);
//    if(hit.getPrimitive()){
//      // Ray hit something...
//      const Material* matl = hit.getMaterial();
//      //matl->shade(result, context, ray, hit, atten, depth);
//      return hit.minT();
//    } else {
//      background->getBackgroundColor(result, context, ray);
//      return DBL_MAX;
//    }
//  }
//}
