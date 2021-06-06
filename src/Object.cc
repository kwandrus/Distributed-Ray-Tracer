
#include "Object.h"

Object::Object()
	: isMoving(false), numMotionSamples(0), motionVelocity(0, 0, 0)
{
}

Object::~Object()
{
}

void Object::preprocess()
{
}
