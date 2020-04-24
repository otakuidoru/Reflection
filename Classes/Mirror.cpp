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
Mirror::Mirror() : Sprite() {
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
Mirror* Mirror::create(b2World* world) {
	Mirror* mirror = new (std::nothrow) Mirror();
	if (mirror && mirror->initWithFile("mirror.png", world)) {
		mirror->autorelease();
		return mirror;
	}
	CC_SAFE_DELETE(mirror);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Mirror::initWithFile(const std::string& filename, b2World* world) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->rotatable = true;
	this->rotating = false;
	this->direction = 0;
	this->reflectionNormal = this->direction + 45.0f;
	this->plane = new Plane(Vec3(std::cosf(this->reflectionNormal)*DEGTORAD, std::cosf(this->reflectionNormal)*DEGTORAD, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));
	this->updateNeeded = false;

	this->setAnchorPoint(Vec2(0.5f, 0.5f));

	//////////////////////////////////////////////////////////////////////
	//
	//  Box2D
	//
	//////////////////////////////////////////////////////////////////////

	// define the body definition

	b2BodyDef bodyDefinition;
	bodyDefinition.position.SetZero();
	auto body = world->CreateBody(&bodyDefinition);

	// define the shape (triangle)

	b2ChainShape shape;
	std::vector<b2Vec2> vertices;
	vertices.push_back(b2Vec2(-0.5f, -0.5f));
	vertices.push_back(b2Vec2(0.5f, -0.5f));
	vertices.push_back(b2Vec2(-0.5f, 0.5f));
	shape.CreateLoop(vertices.data(), vertices.size());

	// define the filter

//	b2Filter filter;
//	filter.categoryBits;
//	filter.maskBits;
//	filter.groupIndex;

	// define the fixture definition

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.userData = nullptr;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.density = 1.0f;
//	fixtureDef.isSensor;
//	fixtureDef.filter = filter;

	this->fixture = body->CreateFixture(&fixtureDef);

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

///**
// *
// */
//void Mirror::rotateCounterclockwise() {
//	this->rotate(false);
//}
//
///**
// *
// */
//void Mirror::rotateClockwise() {
//	this->rotate(true);
//}
//
///**
// *
// */
//void Mirror::rotate(bool clockwise) {
//	if (this->isRotatable()) {
//		if (clockwise) {
//			this->direction = (this->direction - 1) % 4;
//		} else {
//			this->direction = (this->direction + 1) % 4;
//		}
//
//		if (this->direction < 0) {
//			this->direction += 4;
//		}
//
//		switch (this->direction) {
//			case 0: {
//				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 0.0f));
//			} break;
//			case 1: {
//				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 90.0f));
//			} break;
//			case 2: {
//				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 180.0f));
//			} break;
//			case 3: {
//				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 270.0f));
//			} break;
//		}
//	}
//}

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
void Mirror::setPosition(const Vec2& position) {
	Sprite::setPosition(position);
	this->getBox2DBody()->SetTransform(b2Vec2(position.x / Globals::getInstance().getBox2DScale(), position.y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Mirror::setPositionNormalized(const Vec2& position) {
	Sprite::setPositionNormalized(position);
	this->getBox2DBody()->SetTransform(b2Vec2(this->getParent()->getContentSize().width * position.x / Globals::getInstance().getBox2DScale(), this->getParent()->getContentSize().height * position.y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Mirror::setPosition(float x, float y) {
	Sprite::setPosition(x, y);
	this->getBox2DBody()->SetTransform(b2Vec2(x / Globals::getInstance().getBox2DScale(), y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Mirror::setPositionX(float x) {
	Sprite::setPositionX(x);
	this->getBox2DBody()->SetTransform(b2Vec2(x / Globals::getInstance().getBox2DScale(), this->getPositionY() / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Mirror::setPositionY(float y) {
	Sprite::setPositionY(y);
	this->getBox2DBody()->SetTransform(b2Vec2(this->getPositionX() / Globals::getInstance().getBox2DScale(), y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Mirror::setRotation(float rotation) {
	Sprite::setRotation(rotation);
	this->getBox2DBody()->SetTransform(b2Vec2(this->getPositionX() / Globals::getInstance().getBox2DScale(), this->getPositionY() / Globals::getInstance().getBox2DScale()), -rotation*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Mirror::update(float dt) {
	if (this->needsUpdate()) {
		this->reflectionNormal = this->getRotation() + 45.0f;

		if (this->plane) {
			delete this->plane;
		}
		this->plane = new Plane(Vec3(std::cosf(this->getRotation()*DEGTORAD)*RADTODEG, std::sinf(this->getRotation()*DEGTORAD)*RADTODEG, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));

		this->updateNeeded = false;
	}
}

