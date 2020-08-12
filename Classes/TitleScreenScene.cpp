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

#include "AppDelegate.h"
#include "TitleScreenScene.h"
#include "WorldSelectScene.h"
#include "CreditsScreenScene.h"

USING_NS_CC;

/**
 * Print useful error message instead of segfaulting when files are not there.
 */
static void problemLoading(const char* filename) {
	log("Error while loading: %s\n", filename);
	log("Depending on how you compiled you might have to add 'Resources/' in front of filenames in TitleScreenScene.cpp\n");
}

/**
 *
 */
Scene* TitleScreen::createScene() {
	return TitleScreen::create();
}

/**
 * on "init" you need to initialize your instance
 */
bool TitleScreen::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add your codes below...

	auto background = Sprite::create("reflection_title_1536x2048.png");
	background->setPositionNormalized(Vec2(0.5f, 0.5f));
	this->addChild(background, -2);

	auto backgroundText = Sprite::create("reflection_title_text_1536x2048.png");
	const float textScale = visibleSize.width / backgroundText->getContentSize().width;
	backgroundText->setPositionNormalized(Vec2(0.5f, 0.5f));
	backgroundText->setScale(textScale);
	this->addChild(backgroundText, -1);

	std::string journeyPhrase;
	if (!UserDefault::getInstance()->getBoolForKey("launched_before")) {
		journeyPhrase = "Begin Journey";
	} else {
		journeyPhrase = "Continue Journey";
	}

	// create the WorldSelect Label
	this->worldSelectLabel = Label::createWithTTF(journeyPhrase, "fonts/centurygothic.ttf", 160);
	if (worldSelectLabel == nullptr) {
		problemLoading("'fonts/centurygothic.ttf'");
	} else {
		this->worldSelectLabel->setScale(textScale);

		// position the label on the top center of the screen
		this->worldSelectLabel->setPosition(Vec2(768.0f, 768.0f));

		// set the label color to white
		this->worldSelectLabel->setColor(Color3B(255, 255, 255));

		// add the label as a child to this layer
		this->addChild(this->worldSelectLabel, 10);
	}

	// create the info sprite
	this->infoSprite = Sprite::create("info.png");
	this->infoSprite->setPosition(Vec2(
		origin.x + this->infoSprite->getContentSize().width/2.0f,
		origin.y + this->infoSprite->getContentSize().height/2.0f
	));
	this->infoSprite->setScale(visibleSize.width / 1536.0f);
	this->addChild(this->infoSprite, 255);

	// create the reset sprite
	this->resetSprite = Sprite::create("reset.png");
	this->resetSprite->setPosition(Vec2(
		origin.x + visibleSize.width - this->infoSprite->getContentSize().width/2.0f,
		origin.y + this->infoSprite->getContentSize().height/2.0f
	));
	this->resetSprite->setScale(visibleSize.width / 1536.0f);
	this->addChild(this->resetSprite, 255);

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
		if (this->worldSelectLabel->getBoundingBox().containsPoint(touch->getLocation())) {
			consuming = true;
			auto scene = WorldSelect::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
//		} else if (this->infoSprite->getBoundingBox().containsPoint(touch->getLocation())) {
//			consuming = true;
//			auto scene = InfoScreen::createScene();
//			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
//		} else if (this->resetSprite->getBoundingBox().containsPoint(touch->getLocation())) {
//			consuming = true;
//			auto scene = ResetScreen::createScene();
//			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}

		return consuming;
	};

	// triggered when moving touch
	touchListener->onTouchMoved = [](Touch* touch, Event* event) {};

	// triggered when released
	touchListener->onTouchEnded = [](Touch* touch, Event* event) {};

	// add listener
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

