/****************************************************************************
 Copyright (c) 2020 ZenProgramming.com, Ltd.
 
 http://www.zenprogramming.com
 
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
#include "WorldSelectScene.h"
#include "BackArrow.h"

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	log("Error while loading: %s\n", filename);
	log("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

/**
 *
 */
static int worldNameCallback(void* object, int argc, char** data, char** azColName) {
	// 0     1             2,            3,            4
	// name, name_color_r, name_color_g, name_color_b, background_path

	if (LevelSelect* const scene = static_cast<LevelSelect*>(object)) {
		const Size visibleSize = Director::getInstance()->getVisibleSize();
		const Vec2 origin = Director::getInstance()->getVisibleOrigin();

		std::string worldName(data[0]);
		const int worldNameColorR = std::atoi(data[1]);
		const int worldNameColorG = std::atoi(data[2]);
		const int worldNameColorB = std::atoi(data[3]);
		std::string backgroundName(data[4]);

		auto background = Sprite::create(backgroundName);
		const float scale = std::max(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		background->setScale(scale);
		background->setPositionNormalized(Vec2(0.5f, 0.5f));
		scene->addChild(background, -1);

		auto label = Label::createWithTTF(worldName, "fonts/centurygothic_bold.ttf", 160);
		if (label == nullptr) {
			problemLoading("'fonts/centurygothic_bold.ttf'");
		} else {
			// position the label on the top center of the screen
			label->setScale(visibleSize.width / 1536.0f);
			label->setPosition(Vec2(768.0f, origin.y + visibleSize.height - label->getContentSize().height * 2.0f * label->getScale()));
			label->setTextColor(Color4B(worldNameColorR, worldNameColorG, worldNameColorB, 255));

			// add the label as a child to this layer
			scene->addChild(label, 10);
		}
	}

	return 0;
}

/**
 *
 */
static int levelSpriteCallback(void* object, int argc, char** data, char** azColName) {
	// 0   1      2         3          4          5       6          7             8           9              10
	// id, title, world_id, level_num, file_path, locked, num_stars, fastest_time, first_play, next_level_id, level_sprite_path

	if (LevelSelect* const scene = static_cast<LevelSelect*>(object)) {
		const Size visibleSize = Director::getInstance()->getVisibleSize();
		const Vec2 origin = Director::getInstance()->getVisibleOrigin();
		const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

		const int levelId = std::atoi(data[0]);
		const int levelNum = std::atoi(data[3]);
		std::string levelNumFilename(scene->levelNumFilePathMap[levelNum]);
		//log("com.zenprogramming.reflection: level filename num %s", levelNumFilename.c_str());
		std::string levelFilename(data[4]);
		//log("com.zenprogramming.reflection: level filename %s", levelFilename.c_str());

		//log("com.zenprogramming.reflection: position (%f, %f)", position.x, position.y);
		const bool locked = std::atoi(data[5]) == 1;
		const int numStars = std::atoi(data[6]);
		std::string levelSpriteFilename(data[10]);

		// create the background
		auto levelSelectSprite = Sprite::create(levelSpriteFilename);
		const Vec2 position(
			origin.x + (visibleSize.width  / 2.0f) + levelSelectSprite->getContentSize().width * SCALE * scene->levelNumRelativePositionMap[levelNum].x,
			origin.y + (visibleSize.height / 2.0f) + levelSelectSprite->getContentSize().height * SCALE * scene->levelNumRelativePositionMap[levelNum].y
		);
		levelSelectSprite->setPosition(position);
		levelSelectSprite->setScale(SCALE);
		scene->addChild(levelSelectSprite);

		if (locked) {
			auto lockedSprite = Sprite::create("locked.png");
			lockedSprite->setPositionNormalized(Vec2(0.5f, 0.5f));
			levelSelectSprite->addChild(lockedSprite);
		} else {
			auto levelNumSprite = Sprite::create(levelNumFilename);
			levelNumSprite->setPositionNormalized(Vec2(0.5f, 0.5f));
			levelSelectSprite->addChild(levelNumSprite);

			std::string numStarsSpriteFilename;
			if (numStars == 0)      { numStarsSpriteFilename = "0_star.png"; }
			else if (numStars == 1) { numStarsSpriteFilename = "1_star.png"; }
			else if (numStars == 2) { numStarsSpriteFilename = "2_star.png"; }
			else if (numStars == 3) { numStarsSpriteFilename = "3_star.png"; }

			auto numStarsSprite = Sprite::create(numStarsSpriteFilename);
			numStarsSprite->setPositionNormalized(Vec2(0.5f, 0.5f));
			levelSelectSprite->addChild(numStarsSprite);
		}

		scene->levelSprites[levelSelectSprite] = levelFilename;
		scene->levelSpriteLockedMap[levelSelectSprite] = locked;
		scene->levelSpriteLevelIdMap[levelSelectSprite] = levelId;
	}

	return 0;
}

/**
 *
 */
static int numStarsCallback(void* object, int argc, char** data, char** azColName) {
	// 0
	// num_stars

	if (LevelSelect* const scene = static_cast<LevelSelect*>(object)) {
		const Size visibleSize = Director::getInstance()->getVisibleSize();
		const Vec2 origin = Director::getInstance()->getVisibleOrigin();
		const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

		const int numStars = std::atoi(data[0]);

		std::stringstream ss;
		ss << numStars << "/75";
		auto label = Label::createWithTTF(ss.str(), "fonts/centurygothic_bold.ttf", 96);
		if (label == nullptr) {
			problemLoading("'fonts/centurygothic_bold.ttf'");
		} else {
			label->setAnchorPoint(Vec2(1.0f, 0.5f));
			label->setHorizontalAlignment(TextHAlignment::RIGHT);

			// position the label on the top center of the screen
			label->setPosition(Vec2(1496.0f, 100.0f));

			// add the label as a child to this layer
			scene->addChild(label, 255);
		}
	}

	return 0;
}

/**
 *
 */
Scene* LevelSelect::createScene(int worldId) {
	return LevelSelect::create(worldId);
}

/**
 *
 */
LevelSelect* LevelSelect::create(int worldId) {
	LevelSelect* ret = new (std::nothrow) LevelSelect();
	if (ret && ret->init(worldId)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

// on "init" you need to initialize your instance
bool LevelSelect::init(int worldId) {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	this->worldId = worldId;

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

	this->levelNumRelativePositionMap = {
		{  1, Vec2(-2.0f,  2.0f) },
		{  2, Vec2(-1.0f,  2.0f) },
		{  3, Vec2( 0.0f,  2.0f) },
		{  4, Vec2( 1.0f,  2.0f) },
		{  5, Vec2( 2.0f,  2.0f) },
		{  6, Vec2(-2.0f,  1.0f) },
		{  7, Vec2(-1.0f,  1.0f) },
		{  8, Vec2( 0.0f,  1.0f) },
		{  9, Vec2( 1.0f,  1.0f) },
		{ 10, Vec2( 2.0f,  1.0f) },
		{ 11, Vec2(-2.0f,  0.0f) },
		{ 12, Vec2(-1.0f,  0.0f) },
		{ 13, Vec2( 0.0f,  0.0f) },
		{ 14, Vec2( 1.0f,  0.0f) },
		{ 15, Vec2( 2.0f,  0.0f) },
		{ 16, Vec2(-2.0f, -1.0f) },
		{ 17, Vec2(-1.0f, -1.0f) },
		{ 18, Vec2( 0.0f, -1.0f) },
		{ 19, Vec2( 1.0f, -1.0f) },
		{ 20, Vec2( 2.0f, -1.0f) },
		{ 21, Vec2(-2.0f, -2.0f) },
		{ 22, Vec2(-1.0f, -2.0f) },
		{ 23, Vec2( 0.0f, -2.0f) },
		{ 24, Vec2( 1.0f, -2.0f) },
		{ 25, Vec2( 2.0f, -2.0f) }
	};

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float SCALE = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	// create the back arrow
	auto backArrow = BackArrow::create();
	backArrow->setPosition(Vec2(origin.x + backArrow->getContentSize().width / 2.0f, origin.y + backArrow->getContentSize().height / 2.0f));
	backArrow->setScale(visibleSize.width / 1536.0f);
	backArrow->setColor(Color3B(255, 255, 255));
	backArrow->onClick = []() {
		auto worldSelectScene = WorldSelect::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, worldSelectScene, Color3B(0, 0, 0)));
	};
	this->addChild(backArrow, 255);

	/////////////////////////////////////////////////////////////////////////////
	//
	// create the level sprites
	//
	/////////////////////////////////////////////////////////////////////////////

	std::stringstream target;
	target << FileUtils::getInstance()->getWritablePath() << "levels.db";
	//log("com.zenprogramming.reflection: Target Filename = %s", target.str().c_str());

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

	std::stringstream worldNameSs;
	worldNameSs << "SELECT name, name_color_r, name_color_g, name_color_b, background_path FROM game_worlds WHERE id = " << this->worldId;
	rc = sqlite3_exec(db, worldNameSs.str().c_str(), worldNameCallback, static_cast<void*>(this), &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	std::stringstream ss;
	ss << "SELECT game_levels.id, game_levels.title, game_levels.world_id, game_levels.level_num, game_levels.file_path, game_levels.locked, game_levels.num_stars, game_levels.fastest_time, game_levels.first_play, game_levels.next_level_id, game_worlds.level_sprite_path FROM game_levels INNER JOIN game_worlds ON game_levels.world_id = game_worlds.id WHERE world_id = " << this->worldId << " ORDER BY level_num ASC";
	rc = sqlite3_exec(db, ss.str().c_str(), levelSpriteCallback, static_cast<void*>(this), &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	std::stringstream numStarsSs;
	numStarsSs << "SELECT SUM(num_stars) FROM game_levels WHERE world_id = " << this->worldId;
	rc = sqlite3_exec(db, numStarsSs.str().c_str(), numStarsCallback, static_cast<void*>(this), &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}

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
			if (itr->first->getBoundingBox().containsPoint(touch->getLocation()) && !levelSpriteLockedMap[itr->first]) {
				consuming = true;
				auto levelScene = HelloWorld::createScene(itr->second, levelSpriteLevelIdMap[itr->first], this->worldId);
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, levelScene, Color3B(0, 0, 0)));
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

