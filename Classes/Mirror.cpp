/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <cmath>
#include <memory>
#include <utility>
#include "Globals.h"
#include "Mirror.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Mirror::Mirror(int id) : Sprite(), id(id) {
}

/**
 *
 */
Mirror::~Mirror() {
	if (this->plane) {
		delete this->plane;
	}
}

/**
 *
 */
Mirror* Mirror::create(int id) {
	Mirror* mirror = new (std::nothrow) Mirror(id);
	if (mirror && mirror->initWithFile("mirror.png")) {
		mirror->autorelease();
		return mirror;
	}
	CC_SAFE_DELETE(mirror);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Mirror::initWithFile(const std::string& filename) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->rotatable = true;
	this->rotating = false;
	this->direction = Direction::NORTHEAST;
	this->plane = new Plane(Vec3(std::cosf(-45.0f)*DEGTORAD, std::cosf(-45.0f)*DEGTORAD, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), -45.0f));

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Create a "one by one" touch event listener (processes one touch at a time)
  //
  //////////////////////////////////////////////////////////////////////////////

  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->setSwallowTouches(true);

	// triggered when pressed
	touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
		bool consuming = false;

		if (!this->isRotating()) {
			if (this->getBoundingBox().containsPoint(touch->getLocation())) {
				consuming = true;
				this->runAction(Sequence::create(
					CallFunc::create([&]() {
						// set rotating flag
						this->setRotating(true);
					}),
					RotateBy::create(Mirror::ROTATION_TIME, 90.0f),
					CallFunc::create([&]() {
						// correct for bounds outside [0.0f, 360.0f]
						if (this->getRotation() < 0.0f) {
							this->setRotation(this->getRotation() + 360.0f);
						} else if (this->getRotation() >= 360.0f) {
							this->setRotation(this->getRotation() - 360.0f);
						}
						// set not rotating anymore
						this->setRotating(false);
					}),
					nullptr
				));
			}
		}

		return consuming;
	};

	// triggered when moving touch
	touchListener->onTouchMoved = [&](Touch* touch, Event* event) {
	};

	// triggered when released
	touchListener->onTouchEnded = [&](Touch* touch, Event* event) {
	};

	// add listener
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void Mirror::startReflect(Laser* originatingLaser) {
//	this->laser = ;
}

/**
 *
 */
void Mirror::stopReflect(Laser* originatingLaser) {
//	this->laser = ;
}

/**
 *
 */
void Mirror::update(float dt) {
//	this->plane->initPlane(const Vec3& p1, const Vec3& p2, const Vec3& p3);//(Vec3(std::cosf(this->getRotation()*DEGTORAD)*RADTODEG, std::sinf(this->getRotation()*DEGTORAD)*RADTODEG, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), this->reflectionNormal));
}

