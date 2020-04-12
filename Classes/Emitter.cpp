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

/**
 *
 */
Emitter::Emitter() : Node() {
}

/**
 *
 */
Emitter::~Emitter() {
}

/**
 * on "init" you need to initialize your instance
 */
bool Emitter::init() {
	//////////////////////////////
	// 1. super init first
	if (!Node::init()) {
		return false;
	}

	this->direction = 0;
	this->sprite = Sprite::create("emitter.png");

	this->sprite->setPositionNormalized(Vec2(0.5f, 0.5f));
	this->addChild(this->sprite);

	this->setContentSize(this->sprite->getContentSize());
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

		if (this->getBoundingBox().containsPoint(touch->getLocation())) {
			consuming = true;
			this->runAction(Sequence::create(
				CallFunc::create([&]() {
					this->setActive(!this->isActive());
				}),
				nullptr
			));
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

