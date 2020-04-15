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
Mirror* Mirror::create() {
	Mirror* mirror = new (std::nothrow) Mirror();
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
	this->direction = 0;
	this->reflectionNormal = this->direction + 45.0f;
	this->plane = new Plane(Vec3(std::cosf(this->reflectionNormal), std::cosf(this->reflectionNormal), 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));
	this->updateNeeded = false;

	this->setAnchorPoint(Vec2(0.5f, 0.5f));

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
						this->setRotating(true);
					}),
					RotateBy::create(Mirror::ROTATION_TIME, 90.0f),
					CallFunc::create([&]() {
						if (this->getRotation() < 0.0f) {
							this->setRotation(this->getRotation() + 360.0f);
						} else if (this->getRotation() >= 360.0f) {
							this->setRotation(this->getRotation() - 360.0f);
						}
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
void Mirror::rotateCounterclockwise() {
	this->rotate(false);
}

/**
 *
 */
void Mirror::rotateClockwise() {
	this->rotate(true);
}

/**
 *
 */
void Mirror::rotate(bool clockwise) {
	if (this->isRotatable()) {
		if (clockwise) {
			this->direction = (this->direction + 1) % 4;
		} else {
			this->direction = (this->direction - 1) % 4;
		}

		if (this->direction < 0) {
			this->direction += 4;
		}

		switch (this->direction) {
			case 0: {
				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 0.0f));
			} break;
			case 1: {
				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 90.0f));
			} break;
			case 2: {
				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 180.0f));
			} break;
			case 3: {
				this->runAction(RotateTo::create(Mirror::ROTATION_TIME, 270.0f));
			} break;
		}
	}
}

/**
 *
 */
void Mirror::setScaleX(float scaleX) {
	this->updateNeeded = true;
	Sprite::setScaleX(scaleX);
}

/**
 *
 */ 
void Mirror::setScaleY(float scaleY) {
	this->updateNeeded = true;
	Sprite::setScaleY(scaleY);
}

/**
 *
 */
void Mirror::setScale(float scale) {
	this->updateNeeded = true;
	Sprite::setScale(scale);
}

/**
 *
 */
void Mirror::setScale(float scaleX, float scaleY) {
	this->updateNeeded = true;
	Sprite::setScale(scaleX, scaleY);
}

/**
 *
 */
void Mirror::setPosition(const Vec2& position) {
	this->updateNeeded = true;
	Sprite::setPosition(position);
}

/**
 *
 */
void Mirror::setPositionNormalized(const Vec2& position) {
	this->updateNeeded = true;
	Sprite::setPositionNormalized(position);
}

/**
 *
 */
void Mirror::setNormalizedPosition(const Vec2& position) {
	this->updateNeeded = true;
	Sprite::setNormalizedPosition(position);
}

/**
 *
 */
void Mirror::setPosition(float x, float y) {
	this->updateNeeded = true;
	Sprite::setPosition(x, y);
}

/**
 *
 */
void Mirror::setPositionX(float x) {
	this->updateNeeded = true;
	Sprite::setPositionX(x);
}

/**
 *
 */
void Mirror::setPositionY(float y) {
	this->updateNeeded = true;
	Sprite::setPositionY(y);
}

/**
 *
 */
void Mirror::setRotation(float rotation) {
	this->updateNeeded = true;
	Sprite::setRotation(rotation);
}

/**
 *
 */
void Mirror::setRotation3D(const Vec3& rotation) {
	this->updateNeeded = true;
	Sprite::setRotation3D(rotation);
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
		this->plane = new Plane(Vec3(std::cosf(this->reflectionNormal), std::cosf(this->reflectionNormal), 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));

		this->updateNeeded = false;
	}
}

