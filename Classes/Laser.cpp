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
#include "Laser.h"

USING_NS_CC;

/**
 *
 */
Laser::Laser(int id) : Sprite(), id(id) {
}

/**
 *
 */
Laser::~Laser() {
	if (this->ray) {
		delete this->ray;
	}
}

/**
 *
 */
Laser* Laser::create(int id) {
	Laser* laser = new (std::nothrow) Laser(id);
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

	this->setAnchorPoint(Vec2(0.0f, 0.5f));

	this->ray = new Ray(Vec3(std::cosf(this->getRotation()*M_PI/180.0f), std::sinf(this->getRotation()*M_PI/180.0f), 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));
	this->updateNeeded = false;

	return true;
}

/**
 *
 */
void Laser::setScaleX(float scaleX) {
	this->updateNeeded = true;
	Sprite::setScaleX(scaleX);
}

/**
 *
 */ 
void Laser::setScaleY(float scaleY) {
	this->updateNeeded = true;
	Sprite::setScaleY(scaleY);
}

/**
 *
 */
void Laser::setScale(float scale) {
	this->updateNeeded = true;
	Sprite::setScale(scale);
}

/**
 *
 */
void Laser::setScale(float scaleX, float scaleY) {
	this->updateNeeded = true;
	Sprite::setScale(scaleX, scaleY);
}

/**
 *
 */
void Laser::setPosition(const Vec2& position) {
	this->updateNeeded = true;
	Sprite::setPosition(position);
}

/**
 *
 */
void Laser::setPositionNormalized(const Vec2& position) {
	this->updateNeeded = true;
	Sprite::setPositionNormalized(position);
}

/**
 *
 */
void Laser::setNormalizedPosition(const Vec2& position) {
	this->updateNeeded = true;
	Sprite::setNormalizedPosition(position);
}

/**
 *
 */
void Laser::setPosition(float x, float y) {
	this->updateNeeded = true;
	Sprite::setPosition(x, y);
}

/**
 *
 */
void Laser::setPositionX(float x) {
	this->updateNeeded = true;
	Sprite::setPositionX(x);
}

/**
 *
 */
void Laser::setPositionY(float y) {
	this->updateNeeded = true;
	Sprite::setPositionY(y);
}

/**
 *
 */
void Laser::setRotation(float rotation) {
	this->updateNeeded = true;
	Sprite::setRotation(rotation);
}

/**
 *
 */
void Laser::setRotation3D(const Vec3& rotation) {
	this->updateNeeded = true;
	Sprite::setRotation3D(rotation);
}

/**
 *
 */
float Laser::dist(const Plane& plane) const {
	return this->ray->dist(plane);
}

/**
 *
 */
Vec3 Laser::intersects(const Plane& plane) const {
	return this->ray->intersects(plane);
}

/**
 *
 */
void Laser::update(float dt) {
	if (this->needsUpdate()) {
		this->ray->set(Vec3(std::cosf(this->getRotation()*M_PI/180.0f), std::sinf(this->getRotation()*M_PI/180.0f), 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));

		this->updateNeeded = false;
	}
}

