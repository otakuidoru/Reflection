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

	// Box2D configuration

	this->worldNode = b2WorldNode::create();
	//this->setPositionNormalized(Vec2(0.0f, 0.0f));
	this->addChild(this->worldNode, 1);

	/////////////////////////////
	// 2. add your codes below...

	auto background = LayerGradient::create(Color4B(253, 158, 246, 255), Color4B(255, 255, 255, 255), Vec2(1.0f, 1.0f));
	this->addChild(background, -1);

	// add a label that shows "Hello World" and create and initialize a label

	auto label = Label::createWithTTF("Reflection", "fonts/motioncontrol-bold.ttf", 96);
	if (label == nullptr) {
		problemLoading("'fonts/motioncontrol-bold.ttf'");
	} else {
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	// Add the mirrors

	Mirror* mirror = Mirror::create(worldNode->getb2World());
	mirror->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	this->worldNode->addChild(mirror, 1);
	this->mirrors.insert(mirror);

	// Add the emitters

	Emitter* emitter = Emitter::create(1, worldNode->getb2World());
	emitter->setPosition(Vec2(visibleSize.width/2, emitter->getContentSize().height));
	emitter->setRotation(-90.0f);
	emitter->onActivate = [&](Emitter* e) {};
	emitter->onDeactivate = [&](Emitter* e) {};
	this->worldNode->addChild(emitter, 1);
	this->emitters.insert(emitter);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void HelloWorld::update(float dt) {
	//for (auto emitter : this->emitters) {
		//for (auto mirror : this->mirrors) {
			//if (emitter->getLaser()->getBoundingBox().intersects(mirror->getBoundingBox())) {
			//	mirror->reflect();
			//}
		//}
	//}
}

