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

#ifdef COCOS2D_DEBUG
#include <sstream>
#endif

#include <cmath>
#include "Laser.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Laser::Laser() : Sprite() {
}

/**
 *
 */
Laser::~Laser() {
}

/**
 *
 */
Laser* Laser::create(const Color3B& color) {
	Laser* laser = new (std::nothrow) Laser();
	if (laser && laser->initWithFileAndColor("laser.png", color)) {
		laser->autorelease();
		return laser;
	}
	CC_SAFE_DELETE(laser);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Laser::initWithFileAndColor(const std::string& filename, const Color3B& color) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->setColor(color);

	return true;
}

/**
 *
 */
Ray Laser::getRay() const {
	//log("com.zenprogramming.reflection: laser contentsize (%f, %f)", this->getContentSize().width, this->getContentSize().height);
	//log("com.zenprogramming.reflection: laser rotation = %f", this->getRotation());
	const Vec2 origin = this->getParent()->convertToWorldSpace(this->getPosition());
	const Vec3 direction(std::cosf(-this->getRotation()*DEGTORAD), std::sinf(-this->getRotation()*DEGTORAD), 0.0f);

	const Ray ray(Vec3(origin.x, origin.y, 0.0f), direction);
	//log("com.zenprogramming.reflection: laser origin (%f, %f), direction (%f, %f)", origin.x, origin.y, direction.x, direction.y);

	return ray;
}

#ifdef COCOS2D_DEBUG
/**
 *
 */
std::string Laser::str() {
	Ray ray = this->getRay();

	std::stringstream ss;
	ss << "{";
	ss << "\t\"origin\" : {";
	ss << "\t\t\"x\" : " << ray._origin.x << ",";
	ss << "\t\t\"y\" : " << ray._origin.y;
	ss << "\t},";
	ss << "\t\"direction\" : {";
	ss << "\t\t\"x\" : " << ray._direction.x << ",";
	ss << "\t\t\"y\" : " << ray._direction.y << ",";
	ss << "\t\t\"z\" : " << ray._direction.z;
	ss << "\t},";
	ss << "\t\"length\" : " << this->getScaleX();
	ss << "}";

	return ss.str();
}
#endif

