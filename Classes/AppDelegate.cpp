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
#include "AppDelegate.h"
#include "TitleScreenScene.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1536, 2048);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

/**
 *
 */
AppDelegate::AppDelegate() {
}

/**
 *
 */
AppDelegate::~AppDelegate() {
	#if USE_AUDIO_ENGINE
		AudioEngine::end();
	#endif
}

/**
 * If you want a different context, modify the value of glContextAttrs it will affect all platforms
 */
void AppDelegate::initGLContextAttrs() {
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages() {
	return 0; //flag for packages manager
}

void execSQL(sqlite3* const db, const std::string& stmt) {
	char* zErrMsg = 0;

	//log("com.zenprogramming.reflection: %s", stmt.c_str());

	int rc = sqlite3_exec(db, stmt.c_str(), nullptr, nullptr, &zErrMsg);
	if (rc != SQLITE_OK) {
		log("com.zenprogramming.reflection: SQL error: %s", zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

/**
 *
 */
bool AppDelegate::applicationDidFinishLaunching() {
	std::string DB_FILENAME = "levels.db";

	std::stringstream target;
	target << FileUtils::getInstance()->getWritablePath() << DB_FILENAME;

	if (!UserDefault::getInstance()->getBoolForKey("launched_before")) {
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

		std::string stmt;
		std::stringstream ss(FileUtils::getInstance()->getStringFromFile("db.sql"));
		while (std::getline(ss, stmt)) {
			execSQL(db, stmt);
		}

		// close the database
		rc = sqlite3_close(db);

		UserDefault::getInstance()->setBoolForKey("launched_before", true);
	}

	/////////////////////////////////////////////////////////////////////////////

	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			glview = GLViewImpl::createWithRect("Reflection", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
		#else
			glview = GLViewImpl::create("Reflection");
		#endif
			director->setOpenGLView(glview);
	}

	// turn on display FPS
	//director->setDisplayStats(true);

	director->setClearColor(Color4F(0.0f, 0.0f, 0.0f, 1.0f));

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));

	register_all_packages();

	// create a scene. it's an autorelease object
	auto scene = TitleScreen::createScene();

	// run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	#if USE_AUDIO_ENGINE
		AudioEngine::pauseAll();
	#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	#if USE_AUDIO_ENGINE
		AudioEngine::resumeAll();
	#endif
}
