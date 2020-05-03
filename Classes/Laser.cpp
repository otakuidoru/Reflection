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
Laser* Laser::create(int id, const Node* const parent) {
	Laser* laser = new (std::nothrow) Laser(id);
	if (laser && laser->initWithFile("blue_laser.png", parent)) {
		laser->autorelease();
		return laser;
	}
	CC_SAFE_DELETE(laser);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Laser::initWithFile(const std::string& filename, const Node* const parent) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->ray = new Ray(
		Vec3(std::cosf(this->getRotation()*DEGTORAD)*RADTODEG, -std::sinf(this->getRotation()*DEGTORAD)*RADTODEG, 0.0f),
		Vec3(this->getPositionX(), this->getPositionY(), 0.0f)
	);
	this->updateNeeded = false;

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void Laser::setPosition(const Vec2& position) {
	Sprite::setPosition(position);
	this->origin.set(position.x, position.y);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPositionNormalized(const Vec2& position) {
	Sprite::setPositionNormalized(position);
	this->origin.set(position.x, position.y);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPosition(float x, float y) {
	Sprite::setPosition(x, y);
	this->origin.set(x, y);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPositionX(float x) {
	Sprite::setPositionX(x);
	this->origin.set(x, this->getPositionY());
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPositionY(float y) {
	Sprite::setPositionY(y);
	this->origin.set(this->getPositionX(), y);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setRotation(float rotation) {
	Sprite::setRotation(rotation);
	this->ray->set(Vec3(std::cosf(rotation*DEGTORAD)*RADTODEG, -std::sinf(rotation*DEGTORAD)*RADTODEG, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setScaleX(float scaleX) {
	Sprite::setScaleX(scaleX);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setScaleY(float scaleY) {
	Sprite::setScaleY(scaleY);
	this->updateNeeded = true;
}


/**
 *
 */
void Laser::setScale(float scale) {
	Sprite::setScale(scale);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setScale(float scaleX, float scaleY) {
	Sprite::setScale(scaleX, scaleY);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::update(float dt) {
	if (this->needsUpdate()) {
		//this->ray->set(Vec3(std::cosf(this->getRotation()*DEGTORAD)*RADTODEG, -std::sinf(this->getRotation()*DEGTORAD)*RADTODEG, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));

		this->updateNeeded = false;
	}
}

