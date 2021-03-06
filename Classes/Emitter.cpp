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

#include "Emitter.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Emitter::Emitter(int id, ColorType colorType) : GameObject(id, colorType, 4) {
}

/**
 *
 */
Emitter::~Emitter() {
}

/**
 *
 */
Emitter* Emitter::create(int id, ColorType colorType) {
	Emitter* emitter = new (std::nothrow) Emitter(id, colorType);
	if (emitter && emitter->initWithFile("emitter.png")) {
		emitter->autorelease();
		return emitter;
	}
	CC_SAFE_DELETE(emitter);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Emitter::initWithFile(const std::string& filename) {
	//////////////////////////////
	// 1. super init first
	if (!GameObject::initWithFile(filename)) {
		return false;
	}

	this->active = false;
	this->direction = Direction::EAST;

	this->setPlaneReflective(0, false);
	this->setPlaneReflective(1, false);
	this->setPlaneReflective(2, false);
	this->setPlaneReflective(3, false);

//	//////////////////////////////////////////////////////////////////////////////
//	//
//	//  Create a "one by one" touch event listener (processes one touch at a time)
//	//
//	//////////////////////////////////////////////////////////////////////////////
//
//	auto touchListener = EventListenerTouchOneByOne::create();
//	touchListener->setSwallowTouches(true);
//
//	// triggered when pressed
//	touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
//		bool consuming = false;
//
//		if (this->getBoundingBox().containsPoint(touch->getLocation())) {
//			consuming = true;
//			this->setActive(!this->isActive());
//		}
//
//		return consuming;
//	};
//
//	// triggered when moving touch
//	touchListener->onTouchMoved = [&](Touch* touch, Event* event) {
//	};
//
//	// triggered when released
//	touchListener->onTouchEnded = [&](Touch* touch, Event* event) {
//	};
//
//	// add listener
//	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

/**
 *
 */
Plane Emitter::getPlane(unsigned int index) {
	Plane plane;

	const Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
	const Size contentSize = this->getContentSize() * this->getScale();

	switch (index) {	
		case 0: { // first plane - non-reflective
			const float angle = -(this->getRotation() + 45.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
		} break;
		case 1: { // second plane - non-reflective
			const float angle = -(this->getRotation() + 135.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
		} break;
		case 2: { // third plane - non-reflective
			const float angle = -(this->getRotation() + 225.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
		} break;
		case 3: { // fourth plane - non-reflective
			const float angle = -(this->getRotation() + 315.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
		} break;
	}

	return plane;
}

/**
 *
 */
void Emitter::setActive(bool active) {
	this->active = active;
	if (this->active) {
		this->onAfterActivate();
	}
}

