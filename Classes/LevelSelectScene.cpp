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
#include <sqlite3.h>
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

/**
 *
 */
static int callback(void* object, int argc, char** data, char** azColName) {
	if (LevelSelect* const scene = static_cast<LevelSelect*>(object)) {
		const Size visibleSize = Director::getInstance()->getVisibleSize();
		const Vec2 origin = Director::getInstance()->getVisibleOrigin();
		const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

		std::string levelNumFilename(scene->levelNumFilePathMap[std::atoi(data[3])]);
		log("com.zenprogramming.reflection: here level filename num %s", levelNumFilename.c_str());
		std::string levelFilename(data[4]);
		log("com.zenprogramming.reflection: here level filename %s", levelFilename.c_str());
		const float x = std::atof(data[5]);
		log("com.zenprogramming.reflection: here x %f", x);
		const float y = std::atof(data[6]);
		log("com.zenprogramming.reflection: here y %f", y);
		const bool locked = std::atoi(data[7]) == 1;
		const int numStars = std::atoi(data[8]);

		std::string levelSpriteFilename;
		if (locked) {
			levelSpriteFilename = "level_sprite_locked.png";
		} else if (numStars == 0) {
			levelSpriteFilename = "level_sprite_0_stars.png";
		} else if (numStars == 1) {
			levelSpriteFilename = "level_sprite_1_stars.png";
		} else if (numStars == 2) {
			levelSpriteFilename = "level_sprite_2_stars.png";
		} else if (numStars == 3) {
			levelSpriteFilename = "level_sprite_3_stars.png";
		}

		//log("com.zenprogramming.reflection: levelSpriteFilename = %s", levelSpriteFilename.c_str());

		auto levelSelectSprite = Sprite::create(levelSpriteFilename);
		levelSelectSprite->setPosition(Vec2(x, y));
		levelSelectSprite->setScale(SCALE);
		scene->addChild(levelSelectSprite);

		if (!locked) {
			auto levelNumSprite = Sprite::create(levelNumFilename);
			levelNumSprite->setPositionNormalized(Vec2(0.5f, 0.5f));
			levelSelectSprite->setScale(SCALE);
			levelSelectSprite->addChild(levelNumSprite, 10);
		}

		scene->levelSprites[levelSelectSprite] = levelFilename;
	}

	return 0;
}

// on "init" you need to initialize your instance
bool LevelSelect::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	this->levelNumFilePathMap = {
		{  1,  "numbers/1.png" },
		{  2,  "numbers/2.png" },
		{  3,  "numbers/3.png" },
		{  4,  "numbers/4.png" },
		{  5,  "numbers/5.png" },
		{  6,  "numbers/6.png" },
		{  7,  "numbers/7.png" },
		{  8,  "numbers/8.png" },
		{  9,  "numbers/9.png" },
		{ 10, "numbers/10.png" },
		{ 11, "numbers/11.png" },
		{ 12, "numbers/12.png" },
		{ 13, "numbers/13.png" },
		{ 14, "numbers/14.png" },
		{ 15, "numbers/15.png" },
		{ 16, "numbers/16.png" },
		{ 17, "numbers/17.png" },
		{ 18, "numbers/18.png" },
		{ 19, "numbers/19.png" },
		{ 20, "numbers/20.png" },
		{ 21, "numbers/21.png" },
		{ 22, "numbers/22.png" },
		{ 23, "numbers/23.png" },
		{ 24, "numbers/24.png" },
		{ 25, "numbers/25.png" },
	};

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	auto background = Sprite::create("mountain_steps_background.png");
	background->setPositionNormalized(Vec2(0.5f, 0.5f));
	this->addChild(background, -1);

	Director::getInstance()->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 1.0f));

	/////////////////////////////////////////////////////////////////////////////

	// TODO: check for existance of levels.db file first

	std::string DB_FILENAME = "levels.db";

	std::stringstream source;
	source << DB_FILENAME;
	//log("com.zenprogramming.reflection: Source Filename = %s", source.str().c_str());

	std::stringstream target;
	target << FileUtils::getInstance()->getWritablePath() << DB_FILENAME;
	//log("com.zenprogramming.reflection: Target Filename = %s", target.str().c_str());

	// copy the file to the writable area on the device
	FileUtils::getInstance()->writeDataToFile(FileUtils::getInstance()->getDataFromFile(source.str()), target.str());
	//log("com.zenprogramming.reflection: Target Filename = %s", target.str().c_str());

	/////////////////////////////////////////////////////////////////////////////

	// create the level sprites

	sqlite3* db;
	char* zErrMsg = 0;
	int rc;

	// open the database
	rc = sqlite3_open(target.str().c_str(), &db);
	if (rc) {
		log("com.zenprogramming.reflection: Can't open database: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}

	std::string sql = "SELECT id, title, world_id, level_num, file_path, x, y, locked, num_stars, fastest_time FROM levels WHERE world_id = 1";
	rc = sqlite3_exec(db, sql.c_str(), callback, static_cast<void*>(this), &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	//levelNumLockedMap
/*
	std::string sql = "SELECT id, title, world_id, level_num, file_path, x, y, locked, num_stars, fastest_time FROM levels WHERE world_id = 1";
	rc = sqlite3_exec(db, sql.c_str(), callback, static_cast<void*>(this), &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}
*/

	// close the database
	rc = sqlite3_close(db);

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

