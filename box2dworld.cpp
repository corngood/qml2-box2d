/*
 * box2dworld.cpp
 * Copyright (c) 2010-2011 Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright (c) 2011 Joonas Erkinheimo <joonas.erkinheimo@nokia.com>
 * Copyright (c) 2012 Adriano Rezende <atdrez@gmail.com>
 *
 * This file is part of the Box2D QML plugin.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "box2dworld.h"

#include "box2dbody.h"
#include "box2dfixture.h"
#include "box2djoint.h"
#include "box2ddestructionlistener.h"

#include <QTimerEvent>

#include <Box2D.h>

class ContactEvent
{
public:
    enum Type {
        BeginContact,
        EndContact
    };

    Type type;
    Box2DFixture *fixtureA;
    Box2DFixture *fixtureB;
};

class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact *contact);

    void removeEvent(int index) { mEvents.removeAt(index); }
    void clearEvents() { mEvents.clear(); }
    const QList<ContactEvent> &events() { return mEvents; }

private:
    QList<ContactEvent> mEvents;
};

void ContactListener::BeginContact(b2Contact *contact)
{
    ContactEvent event;
    event.type = ContactEvent::BeginContact;
    event.fixtureA = toBox2DFixture(contact->GetFixtureA());
    event.fixtureB = toBox2DFixture(contact->GetFixtureB());
    mEvents.append(event);
}

void ContactListener::EndContact(b2Contact *contact)
{
    ContactEvent event;
    event.type = ContactEvent::EndContact;
    event.fixtureA = toBox2DFixture(contact->GetFixtureA());
    event.fixtureB = toBox2DFixture(contact->GetFixtureB());
    mEvents.append(event);
}


Box2DWorld::Box2DWorld(QQuickItem *parent) :
    QQuickItem(parent),
    mWorld(0),
    mContactListener(new ContactListener),
    mDestructionListener(new Box2DDestructionListener),
    mTimeStep(1.0f / 60.0f),
    mVelocityIterations(10),
    mPositionIterations(10),
    mFrameTime(1000 / 60),
    mGravity(qreal(0), qreal(-10)),
    mIsRunning(true)
{
    connect(mDestructionListener, SIGNAL(fixtureDestroyed(Box2DFixture*)),
            this, SLOT(fixtureDestroyed(Box2DFixture*)));

    const b2Vec2 gravity(mGravity.x(), mGravity.y());

    mWorld = new b2World(gravity);
    mWorld->SetContactListener(mContactListener);
    mWorld->SetDestructionListener(mDestructionListener);
}

Box2DWorld::~Box2DWorld()
{
    delete mWorld;
    delete mContactListener;
    delete mDestructionListener;
}

void Box2DWorld::setRunning(bool running)
{
    if (mIsRunning == running)
        return;

    mIsRunning = running;
    emit runningChanged();

    if (isComponentComplete()) {
        if (running)
            mTimer.start(mFrameTime, this);
        else
            mTimer.stop();
    }
}

void Box2DWorld::setGravity(const QPointF &gravity)
{
    if (mGravity == gravity)
        return;

    mGravity = gravity;
    if (mWorld)
        mWorld->SetGravity(b2Vec2(gravity.x(), gravity.y()));

    emit gravityChanged();
}

void Box2DWorld::componentComplete()
{
    QQuickItem::componentComplete();

    if (mIsRunning)
        mTimer.start(mFrameTime, this);
}

void Box2DWorld::fixtureDestroyed(Box2DFixture *fixture)
{
    QList<ContactEvent> events = mContactListener->events();
    for (int i = events.count() - 1; i >= 0; i--) {
        if (events.at(i).fixtureA == fixture
                || events.at(i).fixtureB == fixture)
            mContactListener->removeEvent(i);
    }
}

void Box2DWorld::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mTimer.timerId()) {
        mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);
        emit stepped();

        // Emit contact signals
        foreach (const ContactEvent &event, mContactListener->events()) {
            switch (event.type) {
            case ContactEvent::BeginContact:
                event.fixtureA->emitBeginContact(event.fixtureB);
                event.fixtureB->emitBeginContact(event.fixtureA);
                break;
            case ContactEvent::EndContact:
                event.fixtureA->emitEndContact(event.fixtureB);
                event.fixtureB->emitEndContact(event.fixtureA);
                break;
            }
        }
        mContactListener->clearEvents();

        // Emit signals for the current state of the contacts
        b2Contact *contact = mWorld->GetContactList();
        while (contact) {
            Box2DFixture *fixtureA = toBox2DFixture(contact->GetFixtureA());
            Box2DFixture *fixtureB = toBox2DFixture(contact->GetFixtureB());

            fixtureA->emitContactChanged(fixtureB);
            fixtureB->emitContactChanged(fixtureA);

            contact = contact->GetNext();
        }
    }

    QQuickItem::timerEvent(event);
}
