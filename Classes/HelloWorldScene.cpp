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

#include "HelloWorldScene.h"

typedef void (*FuncPtrVoidEmitterPtr)(Emitter*);

USING_NS_CC;

/**
 *
 */
Scene* HelloWorld::createScene() {
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	log("Error while loading: %s\n", filename);
	log("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add your codes below...

	auto background = LayerGradient::create(Color4B(253, 158, 246, 255), Color4B(255, 255, 255, 255), Vec2(1.0f, 1.0f));
	this->addChild(background);

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Reflection", "fonts/motioncontrol-bold.ttf", 48);
	if (label == nullptr) {
		problemLoading("'fonts/motioncontrol-bold.ttf'");
	} else {
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		background->addChild(label, 1);
	}

	// Add the mirrors

	Mirror* mirror = Mirror::create();
	mirror->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	background->addChild(mirror);

	// Add the emitters

	Emitter* emitter = Emitter::create();
	emitter->setPosition(Vec2(visibleSize.width/2, emitter->getContentSize().height));
	emitter->setRotation(-90.0f);
	emitter->onActivate = [&](Emitter* e) {
		Laser* laser = Laser::create();
		laser->setPositionNormalized(Vec2(1.0f, 0.5f));
		e->addChild(laser);

		laser->runAction(ScaleTo::create(5.0f, 2048.0f, 1.0f));
	};
	emitter->onDeactivate = [&](Emitter* e) {};
	background->addChild(emitter);

	return true;
}

