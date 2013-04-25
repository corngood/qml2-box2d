#ifndef BOX2DWELDJOINT_H
#define BOX2DWELDJOINT_H

#include "box2djoint.h"
#include <Box2D.h>

class Box2DWeldJoint : public Box2DJoint
{
    Q_OBJECT

public:
    Box2DWeldJoint();

protected:
    b2Joint *createJoint(b2World *world);
    b2WeldJoint *joint() {
        return static_cast<b2WeldJoint*>(Box2DJoint::joint());
    }

private:
    b2WeldJointDef mJointDef;
};

#endif // BOX2DWELDJOINT_H
