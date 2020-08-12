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

#include <cstdlib>
#include <ctime>
#include "BonusStar.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
BonusStar::BonusStar(int id) : GameObject(id, ColorType::NONE, 0) {
}

/**
 *
 */
BonusStar::~BonusStar() {
}

/**
 *
 */
BonusStar* BonusStar::create(int id) {
	BonusStar* bonusStar = new (std::nothrow) BonusStar(id);
	if (bonusStar && bonusStar->initWithFile("bonus_star.png")) {
		bonusStar->autorelease();
		return bonusStar;
	}
	CC_SAFE_DELETE(bonusStar);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool BonusStar::initWithFile(const std::string& filename) {
	//////////////////////////////
	// 1. super init first
	if (!GameObject::initWithFile(filename)) {
		return false;
	}

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	const float scale = visibleSize.width / 1536.0f;

	this->direction = Direction::EAST;

	float angle = ((float)rand()/(float)(RAND_MAX)) * 360.0f;
	this->setRotation(angle);

	this->setScale(0.0f);
	this->runAction(ScaleTo::create(1.0f, 1.1f * scale));
	this->runAction(RepeatForever::create(RotateBy::create(5.0f, 360.0f)));
	this->runAction(RepeatForever::create(
		Sequence::create(
			ScaleTo::create(1.5f, 1.1f * scale),
			ScaleTo::create(1.5f, 0.9f * scale),
			nullptr
		)
	));

	return true;
}

/**
 *
 */
Plane BonusStar::getPlane(unsigned int index) {
	Plane plane;

	return plane;
}

