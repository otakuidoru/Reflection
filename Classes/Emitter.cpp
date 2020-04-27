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
#include "Laser.h"

USING_NS_CC;

/**
 *
 */
Emitter::Emitter(int id, b2World* world) : Sprite(), id(id), world(world) {
}

/**
 *
 */
Emitter::~Emitter() {
}

/**
 *
 */
Emitter* Emitter::create(int id, b2World* world) {
	Emitter* emitter = new (std::nothrow) Emitter(id, world);
	if (emitter && emitter->initWithFile("emitter.png", world)) {
		emitter->autorelease();
		return emitter;
	}
	CC_SAFE_DELETE(emitter);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Emitter::initWithFile(const std::string& filename, b2World* world) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->direction = 0;
	this->active = false;
	this->laser = nullptr;

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
			this->setActive(!this->isActive());
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
void Emitter::setActive(bool active) {
	const bool prevActive = this->active;
	this->active = active;

	if (!prevActive && active) {
		this->laser = Laser::create(this->getId(), this->world, this);
		this->addChild(this->laser);
		this->laser->setAnchorPoint(Vec2(0.0f, 0.5f));
		this->laser->setPositionNormalized(Vec2(1.0f, 0.5f));
		this->laser->runAction(Sequence::create(
			ScaleTo::create(1.0f, 1024.0f, 1.0f),
			CallFunc::create([&]() {
				this->onActivate(this);
			}),
			nullptr
		));
	} else if (prevActive && !active) {
		this->laser->runAction(Sequence::create(
			CallFunc::create([&]() {
				this->laser->setPosition(this->laser->getPosition() + Vec2(this->laser->getScaleX(), 0.0f));
				this->laser->setAnchorPoint(Vec2(1.0f, 0.5f));
			}),
			ScaleTo::create(1.0f, 0.0f, 1.0f),
			RemoveSelf::create(),
			CallFunc::create([&]() {
				this->onDeactivate(this);
			}),
			nullptr
		));
	}
}

