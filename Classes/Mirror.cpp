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
	this->planeReflective[0] = true;
	this->planeReflective[1] = false;
	this->planeReflective[2] = false;

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

		if (this->getBoundingBox().containsPoint(touch->getLocation())) {
			consuming = true;
			this->rotate();
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

	return true;
}

/**
 *
 */
Plane Mirror::getReflectivePlane() {
	return this->getPlane(0);
}

/**
 *
 */
std::vector<Plane> Mirror::getPlanes() {
	std::vector<Plane> planes;

	planes.push_back(this->getPlane(0));
	planes.push_back(this->getPlane(1));
	planes.push_back(this->getPlane(2));

	return planes;
}

/**
 *
 */
Vec3 Mirror::getReflectionVector(const Ray& ray) {
	// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	const Vec3 d = ray._direction;
	const Vec3 n = this->getReflectivePlane().getNormal();
	const Vec3 reflectionVector = d - 2 * (d.dot(n)) * n;
	log("com.zenprogramming.reflection: reflectionVector = (%f, %f, %f)", reflectionVector.x, reflectionVector.y, reflectionVector.z);
	return reflectionVector;
}

/**
 *
 */
Plane Mirror::getPlane(int index) {
	Plane plane;

	const Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
	const Size contentSize = this->getContentSize();

	switch (index) {	
		case 0: { // first plane - reflective
			const float angle = -(this->getRotation() - 45.0f) * DEGTORAD;
			Vec3 pos(worldPos.x, worldPos.y, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
			//log("com.zenprogramming.reflection: mirror[%d] plane[0].pos = (%f, %f, %f)", this->getId(), pos.x, pos.y, pos.z);
			//log("com.zenprogramming.reflection: mirror[%d] plane[0].normal = (%f, %f, %f)", this->getId(), plane.getNormal().x, plane.getNormal().y, plane.getNormal().z);
		} break;
		case 1: { // second plane - non-reflective
			const float angle = -(this->getRotation() + 90.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
			//log("com.zenprogramming.reflection: mirror[%d] plane[1].pos = (%f, %f, %f)", this->getId(), pos.x, pos.y, pos.z);
			//log("com.zenprogramming.reflection: mirror[%d] plane[1].normal = (%f, %f, %f)", this->getId(), plane.getNormal().x, plane.getNormal().y, plane.getNormal().z);
		} break;
		case 2: { // third plane - non-reflective
			const float angle = -(this->getRotation() + 180.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
			//log("com.zenprogramming.reflection: mirror[%d] plane[2].pos = (%f, %f, %f)", this->getId(), pos.x, pos.y, pos.z);
			//log("com.zenprogramming.reflection: mirror[%d] plane[2].normal = (%f, %f, %f)", this->getId(), plane.getNormal().x, plane.getNormal().y, plane.getNormal().z);
		} break;
	}

	return plane;
}

/**
 *
 */
void Mirror::rotate() {
	if (this->isRotatable()) {
		if (!this->isRotating()) {
			this->runAction(Sequence::create(
				// before rotation
				CallFunc::create([&]() {
					log("com.zenprogramming.reflection: BEGIN MIRROR[%d] ROTATION", this->getId());
					this->setRotating(true);
				}),
				// on rotation
				RotateBy::create(Mirror::ROTATION_TIME, 90.0f),
				// after rotation
				CallFunc::create([&]() {
					log("com.zenprogramming.reflection: END MIRROR[%d] ROTATION", this->getId());
					// correct for bounds outside [0.0f, 360.0f]
					if (this->getRotation() < 0.0f) {
						this->setRotation(this->getRotation() + 360.0f);
					} else if (this->getRotation() >= 360.0f) {
						this->setRotation(this->getRotation() - 360.0f);
					}
					switch (this->getDirection()) {
						case Direction::NORTH:     { this->setDirection(Direction::EAST);      } break;
						case Direction::NORTHEAST: { this->setDirection(Direction::SOUTHEAST); } break;
						case Direction::EAST:      { this->setDirection(Direction::SOUTH);     } break;
						case Direction::SOUTHEAST: { this->setDirection(Direction::SOUTHWEST); } break;
						case Direction::SOUTH:     { this->setDirection(Direction::WEST);      } break;
						case Direction::SOUTHWEST: { this->setDirection(Direction::NORTHWEST); } break;
						case Direction::WEST:      { this->setDirection(Direction::NORTH);     } break;
						case Direction::NORTHWEST: { this->setDirection(Direction::NORTHEAST); } break;
					}
					this->setRotating(false);
				}),
				nullptr
			));
		}
	}
}

