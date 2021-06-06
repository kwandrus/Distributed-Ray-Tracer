
#include "Group.h"
#include "BoundingBox.h"
using namespace std;

Group::Group()
{
}

Group::~Group()
{
}

void Group::addObject(Object* object)
{
  objects.push_back(object);
}

void Group::preprocess()
{
  Object*const* begin = &objects[0];
  Object*const* end = &objects[0]+objects.size();
  while (begin != end)
    (*begin++)->preprocess();
}

void Group::getBounds(BoundingBox& bbox) const
{
  Object*const* begin = &objects[0];
  Object*const* end = &objects[0]+objects.size();
  while (begin != end)
    (*begin++)->getBounds(bbox);
}

void Group::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
  Object*const* begin = &objects[0];
  Object*const* end = &objects[0]+objects.size();
  while (begin != end)
    (*begin++)->intersect(hit, context, ray);
}

void Group::setGroupMotion() {
    Object* const* begin = &objects[0];
    Object* const* end = &objects[0] + objects.size();
    while (begin != end) {
        if ((*begin)->getMotion()) {
            // set group object to moving if one of the objects in this group is moving
            setMotion();
            numMotionSamples = (*begin)->getMotionSamples();
            return;
        }
        *begin++;
    }
}

void Group::moveObjects(double time) {
    //Object** begin = &objects[0];
    //Object** end = &objects[0] + objects.size();
    //while (begin != end) {
    for (int i = 0; i < int(objects.size()); i++) {
        if (objects[i]->getMotion()) {
            objects[i]->move(time);
        }
    }
    
}
