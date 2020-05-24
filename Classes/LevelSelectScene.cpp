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

#include <sstream>
#include "LevelSelectScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

/**
 *
 */
Scene* LevelSelect::createScene() {
	return LevelSelect::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	log("Error while loading: %s\n", filename);
	log("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LevelSelect::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float scale = std::min(visibleSize.width / 1536.0f, visibleSize.height / 2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	auto background = LayerGradient::create(Color4B(253, 158, 246, 255), Color4B(255, 255, 255, 255), Vec2(1.0f, 1.0f));
	this->addChild(background);

//	// add a label that shows "Hello World" and create and initialize a label
//	auto label = Label::createWithTTF("Reflection", "fonts/motioncontrol-bold.ttf", 96);
//	if (label == nullptr) {
//		problemLoading("'fonts/motioncontrol-bold.ttf'");
//	} else {
//		// position the label on the top center of the screen
//		label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
//
//		// add the label as a child to this layer
//		this->addChild(label, 1);
//	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	// triggered when pressed
	touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
		bool consuming = false;
		for (std::map<cocos2d::Sprite*, std::string>::iterator itr=levelSprites.begin(); itr!=levelSprites.end(); ++itr) {
			if (itr->first->getBoundingBox().containsPoint(touch->getLocation())) {
				consuming = true;
				auto levelScene = HelloWorld::createScene(itr->second);
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, levelScene, Color3B(0, 0, 0)));
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

	this->createLevelSprite("level_select_1.png", Vec2(origin.x + visibleSize.width / 2.0f - 512.0f * scale, origin.y + visibleSize.height - 512.0f * scale), 1, "1.xml");
	this->createLevelSprite("level_select_1.png", Vec2(origin.x + visibleSize.width / 2.0f,                  origin.y + visibleSize.height - 512.0f * scale), 2, "2.xml");
	this->createLevelSprite("level_select_1.png", Vec2(origin.x + visibleSize.width / 2.0f + 512.0f * scale, origin.y + visibleSize.height - 512.0f * scale), 3, "3.xml");

	return true;
}

/**
 *
 */
void LevelSelect::createLevelSprite(const std::string& levelSpriteFilename, const Vec2& position, int num, const std::string& levelFilename) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float scale = std::min(visibleSize.width / 1536.0f, visibleSize.height / 2048.0f);

	auto levelSelectSprite = Sprite::create(levelSpriteFilename);
	levelSelectSprite->setPosition(position);
	levelSelectSprite->setScale(scale);
	this->addChild(levelSelectSprite);

	std::stringstream ss;
	ss << num;
	auto label = Label::createWithTTF(ss.str(), "fonts/agentorange.ttf", 160);
	label->setPositionNormalized(Vec2(0.5f, 0.5f));
	label->setColor(Color3B(0, 0, 0));
	levelSelectSprite->addChild(label);

	this->levelSprites[levelSelectSprite] = levelFilename;
}

