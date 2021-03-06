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
#include "Portal.h"
#include "ColorType.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Portal::Portal(int id) : GameObject(id, ColorType::NONE, 4) {
}

/**
 *
 */
Portal::~Portal() {
}

/**
 *
 */
Portal* Portal::create(int id) {
	Portal* portal = new (std::nothrow) Portal(id);
	if (portal && portal->initWithFile("portal.png")) {
		portal->autorelease();
		return portal;
	}
	CC_SAFE_DELETE(portal);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Portal::initWithFile(const std::string& filename) {
	//////////////////////////////
	// 1. super init first
	if (!GameObject::initWithFile(filename)) {
		return false;
	}

	this->direction = Direction::EAST;
	this->setPlaneReflective(0, false);
	this->setPlaneReflective(1, false);
	this->setPlaneReflective(2, false);
	this->setPlaneReflective(3, false);

	return true;
}

/**
 *
 */
Plane Portal::getPlane(unsigned int index) {
	Plane plane;

	const Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
	const Size contentSize = this->getContentSize() * this->getScale();

	switch (index) {	
		case 0: { // first plane - non-reflective
			const float angle = -(this->getRotation() - 45.0f) * DEGTORAD;
			Vec3 pos(worldPos.x, worldPos.y, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
			//log("com.zenprogramming.reflection: mirror[%d] plane[0]: pos = (%f, %f, %f) normal = (%f, %f, %f)", this->getId(), pos.x, pos.y, pos.z, plane.getNormal().x, plane.getNormal().y, plane.getNormal().z);
		} break;
		case 1: { // second plane - non-reflective
			const float angle = -(this->getRotation() + 90.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
			//log("com.zenprogramming.reflection: mirror[%d] plane[1]: pos = (%f, %f, %f) normal = (%f, %f, %f)", this->getId(), pos.x, pos.y, pos.z, plane.getNormal().x, plane.getNormal().y, plane.getNormal().z);
		} break;
		case 2: { // third plane - non-reflective
			const float angle = -(this->getRotation() + 180.0f) * DEGTORAD;
			Vec3 pos(worldPos.x + std::cosf(angle) * contentSize.width / 2.0f, worldPos.y + std::sinf(angle) * contentSize.height / 2.0f, 0.0f);
			plane = Plane(Vec3(std::cosf(angle), std::sinf(angle), 0.0f), pos);
			//log("com.zenprogramming.reflection: mirror[%d] plane[2]: pos = (%f, %f, %f) normal = (%f, %f, %f)", this->getId(), pos.x, pos.y, pos.z, plane.getNormal().x, plane.getNormal().y, plane.getNormal().z);
		} break;
		case 3: { // third plane - non-reflective
			// TODO
		} break;
	}

	return plane;
}

