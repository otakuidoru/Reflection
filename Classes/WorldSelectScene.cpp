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

#include <sqlite3.h>
#include "ui/CocosGUI.h"
#include "WorldSelectScene.h"
#include "LevelSelectScene.h"
#include "TitleScreenScene.h"
#include "BackArrow.h"

USING_NS_CC;

/**
 *
 */
static int numWorldsSelectCallback(void* object, int count, char** data, char** azColName) {
	if (ui::ScrollView* const scrollView = static_cast<ui::ScrollView*>(object)) {
		const Size visibleSize = Director::getInstance()->getVisibleSize();
		const int numLevels = std::atoi(data[0]);
		//log("com.zenprogramming.reflection: numLevels %i", numLevels);

		const float contentWidth = visibleSize.width;
		const float contentHeight = std::min(visibleSize.height, numLevels*386.0f);
		const Size contentSize(contentWidth, contentHeight);
		//log("com.zenprogramming.reflection: size %f, %f", contentWidth, contentHeight);

		scrollView->setContentSize(contentSize);
		scrollView->setInnerContainerSize(Size(visibleSize.width, numLevels*386.0f));
	}

	return 0;
}

/**
 *
 */
static int worldSelectCallback(void* object, int count, char** data, char** azColName) {
	// 0   1     2          3
	// id, name, file_path, total_rows

	if (ui::ScrollView* const scrollView = static_cast<ui::ScrollView*>(object)) {
		const int levelId = std::atoi(data[0]);
		std::string name(data[1]);
		std::string filepath(data[2]);
		const int totalRows = std::atoi(data[3]);

		Vec2 position(768.0f, 193.0f*((levelId-1)*2+1));
		//log("com.zenprogramming.reflection %f, %f", position.x, position.y);
		//log("com.zenprogramming.reflection %s", name.c_str());

		auto sprite = ui::ImageView::create(data[2]);
		sprite->setScale(0.95f);
		sprite->setPosition(Vec2(768.0f, 193.0f*((totalRows-levelId)*2+1)));
		auto label = Label::createWithTTF(name, "fonts/centurygothic.ttf", 160);
		label->setPositionNormalized(Vec2(0.5f, 0.5f));
		sprite->addChild(label);
		scrollView->addChild(sprite, 10);

		WorldSelect* scene = static_cast<WorldSelect*>(scrollView->getParent());
		scene->worldSpritesLevelMap[sprite] = levelId;

		sprite->setTouchEnabled(true);
		sprite->setPropagateTouchEvents(true);
		sprite->addTouchEventListener([&](Ref* ref, ui::Widget::TouchEventType type) {
			WorldSelect* scene = static_cast<WorldSelect*>(Director::getInstance()->getRunningScene());
			switch (type) {
				case ui::Widget::TouchEventType::BEGAN: {
				} break;
				case ui::Widget::TouchEventType::MOVED: {
				} break;
				case ui::Widget::TouchEventType::ENDED: {
					auto levelSelectScene = LevelSelect::createScene(scene->worldSpritesLevelMap[ref]);
					Director::getInstance()->replaceScene(TransitionFade::create(0.5f, levelSelectScene, Color3B(0, 0, 0)));
				} break;
				case ui::Widget::TouchEventType::CANCELED: {
				} break;
			}
		});
	}

	return 0;
}

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

	auto background = Sprite::create("reflection_no_title.png");
	background->setPositionNormalized(Vec2(0.5f, 0.5f));
	this->addChild(background, -1);

	// create the back arrow
	auto backArrow = BackArrow::create();
	backArrow->onClick = []() {
		auto titleScreenScene = TitleScreen::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, titleScreenScene, Color3B(0, 0, 0)));
	};
	backArrow->setPosition(Vec2(116.0f, 100.0f));
	this->addChild(backArrow, 255);

	//////////////////////////////////////////////////////////////////////////////
	//
	//  
	//
	//////////////////////////////////////////////////////////////////////////////

	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setSwallowTouches(false);
	scrollView->setBounceEnabled(true);
	scrollView->setAnchorPoint(Vec2(0.5f, 0.5f));
	scrollView->setPosition(Vec2(visibleSize.width/2+origin.x, visibleSize.height/2+origin.y));
	this->addChild(scrollView);

	std::stringstream target;
	target << FileUtils::getInstance()->getWritablePath() << "levels.db";
	//log("com.zenprogramming.reflection: Target Filename = %s", target.str().c_str());

	sqlite3* db;
	char* zErrMsg = 0;
	int rc;

	// open the database
	rc = sqlite3_open(target.str().c_str(), &db);
	if (rc) {
		//log("com.zenprogramming.reflection: Can't open database: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}

	std::string countWorldsSql = "SELECT COUNT(*) FROM game_worlds WHERE locked = 0";
	rc = sqlite3_exec(db, countWorldsSql.c_str(), numWorldsSelectCallback, static_cast<void*>(scrollView), &zErrMsg);
	if (rc != SQLITE_OK) {
		//log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	std::string selectAllSql = "WITH result AS (SELECT id, name, file_path FROM game_worlds WHERE locked = 0 ORDER BY id DESC) SELECT *, (SELECT COUNT(*) FROM result) AS total_rows FROM result";
	rc = sqlite3_exec(db, selectAllSql.c_str(), worldSelectCallback, static_cast<void*>(scrollView), &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: W1 SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	// close the database
	rc = sqlite3_close(db);

	scrollView->jumpToTop();

	return true;
}

