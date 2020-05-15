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
#include "LaserPart.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
LaserPart::LaserPart() : Sprite() {
}

/**
 *
 */
LaserPart::~LaserPart() {
}

/**
 *
 */
LaserPart* LaserPart::create() {
	LaserPart* laserPart = new (std::nothrow) LaserPart();
	if (laserPart && laserPart->initWithFile("red_laser.png")) {
		laserPart->autorelease();
		return laserPart;
	}
	CC_SAFE_DELETE(laserPart);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool LaserPart::initWithFile(const std::string& filename) {
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
Ray LaserPart::getRay() const {
	//log("com.zenprogramming.reflection: laser contentsize (%f, %f)", this->getContentSize().width, this->getContentSize().height);
	const Vec2 origin = this->getParent()->convertToWorldSpace(this->getPosition());
	const Vec3 direction(std::cosf(-this->getRotation()*DEGTORAD), std::sinf(-this->getRotation()*DEGTORAD), 0.0f);

	const Ray ray(Vec3(origin.x, origin.y, 0.0f), direction);
	//log("com.zenprogramming.reflection: laser origin (%f, %f), direction (%f, %f)", origin.x, origin.y, direction.x, direction.y);

	return ray;
}

