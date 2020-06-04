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

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	auto background = Sprite::create("mountain_steps_background.png");
	background->setPositionNormalized(Vec2(0.5f, 0.5f));
	this->addChild(background, -1);

	Director::getInstance()->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 1.0f));

	// create the level sprites

	const float START_X = 168.0f;
	const float START_Y = origin.y + visibleSize.height - 512.0f;
	const float SPACER = 300.0f;

	this->addLevelSprite("level_sprite_0_stars.png", Vec2((START_X + (0 * SPACER)) * SCALE, (START_Y - (0 * SPACER)) * SCALE), SCALE, "1",  "1.xml", "Start Simply");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (1 * SPACER)) * SCALE, (START_Y - (0 * SPACER)) * SCALE), SCALE, "",  "2.xml", "...On The Wall");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (2 * SPACER)) * SCALE, (START_Y - (0 * SPACER)) * SCALE), SCALE, "",  "3.xml", "Three");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (3 * SPACER)) * SCALE, (START_Y - (0 * SPACER)) * SCALE), SCALE, "",  "4.xml", "Obstacle");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (4 * SPACER)) * SCALE, (START_Y - (0 * SPACER)) * SCALE), SCALE, "",  "5.xml", "Five");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (0 * SPACER)) * SCALE, (START_Y - (1 * SPACER)) * SCALE), SCALE, "",  "6.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (1 * SPACER)) * SCALE, (START_Y - (1 * SPACER)) * SCALE), SCALE, "",  "7.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (2 * SPACER)) * SCALE, (START_Y - (1 * SPACER)) * SCALE), SCALE, "",  "8.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (3 * SPACER)) * SCALE, (START_Y - (1 * SPACER)) * SCALE), SCALE, "",  "9.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (4 * SPACER)) * SCALE, (START_Y - (1 * SPACER)) * SCALE), SCALE, "", "10.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (0 * SPACER)) * SCALE, (START_Y - (2 * SPACER)) * SCALE), SCALE, "", "11.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (1 * SPACER)) * SCALE, (START_Y - (2 * SPACER)) * SCALE), SCALE, "", "12.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (2 * SPACER)) * SCALE, (START_Y - (2 * SPACER)) * SCALE), SCALE, "", "13.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (3 * SPACER)) * SCALE, (START_Y - (2 * SPACER)) * SCALE), SCALE, "", "14.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (4 * SPACER)) * SCALE, (START_Y - (2 * SPACER)) * SCALE), SCALE, "", "15.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (0 * SPACER)) * SCALE, (START_Y - (3 * SPACER)) * SCALE), SCALE, "", "16.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (1 * SPACER)) * SCALE, (START_Y - (3 * SPACER)) * SCALE), SCALE, "", "17.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (2 * SPACER)) * SCALE, (START_Y - (3 * SPACER)) * SCALE), SCALE, "", "18.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (3 * SPACER)) * SCALE, (START_Y - (3 * SPACER)) * SCALE), SCALE, "", "19.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (4 * SPACER)) * SCALE, (START_Y - (3 * SPACER)) * SCALE), SCALE, "", "20.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (0 * SPACER)) * SCALE, (START_Y - (4 * SPACER)) * SCALE), SCALE, "", "21.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (1 * SPACER)) * SCALE, (START_Y - (4 * SPACER)) * SCALE), SCALE, "", "22.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (2 * SPACER)) * SCALE, (START_Y - (4 * SPACER)) * SCALE), SCALE, "", "23.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (3 * SPACER)) * SCALE, (START_Y - (4 * SPACER)) * SCALE), SCALE, "", "24.xml", "");
	this->addLevelSprite("level_sprite_locked.png",  Vec2((START_X + (4 * SPACER)) * SCALE, (START_Y - (4 * SPACER)) * SCALE), SCALE, "", "25.xml", "");

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
		for (std::map<cocos2d::Sprite*, std::string>::iterator itr=levelSprites.begin(); itr!=levelSprites.end(); ++itr) {
			if (itr->first->getBoundingBox().containsPoint(touch->getLocation())) {
				consuming = true;
				auto levelScene = HelloWorld::createScene(itr->second);
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, levelScene, Color3B(0, 0, 0)));
				break;
			}
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

/**
 *
 */
void LevelSelect::addLevelSprite(const std::string& levelSpriteFilename, const Vec2& position, float scale, const std::string& num, const std::string& levelFilename, const std::string& levelTitle) {
	auto levelSelectSprite = Sprite::create(levelSpriteFilename);
	levelSelectSprite->setPosition(position);
	levelSelectSprite->setScale(scale);
	this->addChild(levelSelectSprite);

	auto label = Label::createWithTTF(num, "fonts/agentorange.ttf", 96);
	label->setPositionNormalized(Vec2(0.5f, 0.5f));
	label->setColor(Color3B(0, 0, 0));
	levelSelectSprite->addChild(label, 10);
	this->levelSprites[levelSelectSprite] = levelFilename;
}

