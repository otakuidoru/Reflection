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

#include "ui/CocosGUI.h"
#include "WorldSelectScene.h"
#include "LevelSelectScene.h"

USING_NS_CC;

/**
 *
 */
Scene* WorldSelect::createScene() {
	return WorldSelect::create();
}

/**
 * On "init" you need to initialize your instance
 */
bool WorldSelect::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	std::vector<std::string> levelNames { "Astral Plane", "Deep Space", "Zen Garden", "Restful Mountain", "Placid Ocean", "Calming River", "Quiet Meadow", "Tranquil Forest" };
	const float deltaY = 193.0f;//2048.0f / (levelNames.size() * 2);

	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(visibleSize);
	scrollView->setInnerContainerSize(Size(visibleSize.width, levelNames.size()*2*deltaY));
	scrollView->setBackGroundImage("reflection_with_title.png");
	scrollView->setBounceEnabled(true);
	scrollView->setAnchorPoint(Vec2(0.5f, 0.5f));
	scrollView->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(scrollView);

	for (unsigned int i=0; i<levelNames.size(); ++i) {
		auto sprite = Sprite::create("worlds/forest/level_select.png");
		sprite->setScale(0.95f);
		sprite->setPosition(Vec2(768.0f, deltaY * (i * 2 + 1)));
		auto label = Label::createWithTTF(levelNames[i], "fonts/centurygothic.ttf", 160);
		label->setPositionNormalized(Vec2(0.5f, 0.5f));
		sprite->addChild(label);
		scrollView->addChild(sprite, 10);
	}

	scrollView->jumpToTop();

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
//		for (std::map<cocos2d::Sprite*, std::string>::iterator itr=sprites.begin(); itr!=sprites.end(); ++itr) {
//			if (itr->first->getBoundingBox().containsPoint(touch->getLocation()) && !levelSpriteLockedMap[itr->first]) {
				consuming = true;
//				auto levelSelectScene = LevelSelect::createScene(itr->second, levelSpriteLevelIdMap[itr->first]);
//				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, levelSelectScene, Color3B(0, 0, 0)));
//				break;
//			}
//		}

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

