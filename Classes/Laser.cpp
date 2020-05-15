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
#include "Globals.h"
#include "Laser.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Laser::Laser(EmissionSource* const emissionSource) : Sprite(), emissionSource(emissionSource) {
}

/**
 *
 */
Laser::~Laser() {
}

/**
 *
 */
Laser* Laser::create(EmissionSource* const emissionSource) {
	Laser* laser = new (std::nothrow) Laser(emissionSource);
	if (laser && laser->initWithFile("blue_laser.png")) {
		laser->autorelease();
		return laser;
	}
	CC_SAFE_DELETE(laser);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Laser::initWithFile(const std::string& filename) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	return true;
}

/**
 *
 */
Ray Laser::getRay() const {
//	log("com.zenprogramming.reflection: laser[%d] contentsize (%f, %f)", this->getId(), this->getContentSize().width, this->getContentSize().height);
	const Vec2 origin = this->convertToWorldSpace(Vec2::ZERO) - Vec2(this->getContentSize().height/2.0f, 0.0f);
	const Vec3 direction(std::cosf(this->getParent()->getRotation()*DEGTORAD), -std::sinf(this->getParent()->getRotation()*DEGTORAD), 0.0f);

	const Ray ray(Vec3(origin.x, origin.y, 0.0f), direction);
//	log("com.zenprogramming.reflection: laser[%d] origin (%f, %f), direction (%f, %f)", this->getId(), origin.x, origin.y, direction.x, direction.y);

	return ray;
}

