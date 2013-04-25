#include "box2dweldjoint.h"

#include "box2dbody.h"

Box2DWeldJoint::Box2DWeldJoint()
{
}

b2Joint *Box2DWeldJoint::createJoint(b2World *world)
{
    mJointDef.Initialize(bodyA()->body(), bodyB()->body(), 0.5f * (bodyA()->body()->GetWorldCenter() + bodyA()->body()->GetWorldCenter()));
    mJointDef.collideConnected = collideConnected();

    return world->CreateJoint(&mJointDef);
}
