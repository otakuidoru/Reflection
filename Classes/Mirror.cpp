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

#include "Mirror.h"

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

USING_NS_CC;

/**
 *
 */
Mirror::Mirror() : Node() {
}

/**
 *
 */
Mirror::~Mirror() {
}

/**
 * on "init" you need to initialize your instance
 */
bool Mirror::init() {
	//////////////////////////////
	// 1. super init first
	if (!Node::init()) {
		return false;
	}

	this->rotatable = true;
	this->rotating = false;
	this->direction = 0;
	this->sprite = Sprite::create("mirror.png");

	this->sprite->setPositionNormalized(Vec2(0.5f, 0.5f));
	this->addChild(this->sprite);

	this->setContentSize(this->sprite->getContentSize());
	this->setAnchorPoint(Vec2(0.33f, 0.33f));

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

	return true;
}

/**
 *
 */
void Mirror::rotateCounterclockwise() {
	rotate(false);
}

/**
 *
 */
void Mirror::rotateClockwise() {
	rotate(true);
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
				this->sprite->runAction(RotateTo::create(Mirror::ROTATION_TIME, 0.0f));
			} break;
			case 1: {
				this->sprite->runAction(RotateTo::create(Mirror::ROTATION_TIME, 90.0f));
			} break;
			case 2: {
				this->sprite->runAction(RotateTo::create(Mirror::ROTATION_TIME, 180.0f));
			} break;
			case 3: {
				this->sprite->runAction(RotateTo::create(Mirror::ROTATION_TIME, 270.0f));
			} break;
		}
	}
}

/**
 *
 */
void Mirror::update(float dt) {
}

