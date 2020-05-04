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

#include <cmath>
#include "HelloWorldScene.h"
#include "Laser.h"

#define BACKGROUND_LAYER -1
#define LASER_LAYER 1
#define EMITTER_LAYER 2
#define MIRROR_LAYER 2

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

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

	Mirror* mirror1 = Mirror::create(1);
	mirror1->setPosition(Vec2(768, 1024));
	this->addChild(mirror1, MIRROR_LAYER);
	this->mirrors.insert(mirror1);

	Mirror* mirror2 = Mirror::create(2);
	mirror2->setPosition(Vec2(512, 1024));
	this->addChild(mirror2, MIRROR_LAYER);
	this->mirrors.insert(mirror2);

	Mirror* mirror3 = Mirror::create(3);
	mirror3->setPosition(Vec2(768, 1280));
	this->addChild(mirror3, MIRROR_LAYER);
	this->mirrors.insert(mirror3);

	// Add the emitters

	Emitter* emitter = Emitter::create(1);
	emitter->setPosition(Vec2(visibleSize.width/2, emitter->getContentSize().height));
	emitter->setRotation(-90.0f);
	emitter->setDirection(Direction::NORTH);
	emitter->onActivate = [&](Emitter* e) {};
	emitter->onDeactivate = [&](Emitter* e) {};
	this->addChild(emitter, EMITTER_LAYER);
	this->emitters.insert(emitter);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void HelloWorld::update(float dt) {
	for (auto emitter : this->emitters) {
		if (emitter->getLaser()) {
			Mirror* closestMirror = nullptr;
			float minDistance = 2560.0f;

			for (auto mirror : this->mirrors) {
				// get the plane
				const Vec2 mirrorPos = mirror->getParent()->convertToWorldSpace(mirror->getPosition());
				//log("com.zenprogramming.reflection: mirror[%d] is located at (%f, %f)", mirror->getId(), mirrorPos.x, mirrorPos.y);
				const Size mirrorSize = mirror->getContentSize();
				const float halfMirrorWidth = mirrorSize.width / 2.0f;
				const float halfMirrorHeight = mirrorSize.height / 2.0f;
				const float mirrorRotation = mirror->getRotation() * DEGTORAD;
				const float ninetyDegrees = 90.0f * DEGTORAD;
				const Vec3 points[3] = {
					Vec3(mirrorPos.x + std::cosf(mirrorRotation)                 * halfMirrorWidth, mirrorPos.y + std::sinf(mirrorRotation)                 * halfMirrorHeight, 0.0f),
					Vec3(mirrorPos.x + std::cosf(mirrorRotation - ninetyDegrees) * halfMirrorWidth, mirrorPos.y + std::sinf(mirrorRotation + ninetyDegrees) * halfMirrorHeight, 0.0f),
					Vec3(mirrorPos.x + std::cosf(mirrorRotation - ninetyDegrees) * halfMirrorWidth, mirrorPos.y + std::sinf(mirrorRotation + ninetyDegrees) * halfMirrorHeight, 1.0f),
				};
				Plane mirrorPlane(points[0], points[1], points[2]);
				//log("com.zenprogramming.reflection: plane[0] = (%f, %f, %f)", points[0].x, points[0].y, points[0].z);
				//log("com.zenprogramming.reflection: plane[1] = (%f, %f, %f)", points[1].x, points[1].y, points[1].z);
				//log("com.zenprogramming.reflection: plane[2] = (%f, %f, %f)", points[2].x, points[2].y, points[2].z);

				// get the laser ray
				Vec2 emitterRayOrigin = emitter->getParent()->convertToWorldSpace(emitter->getPosition());
				Vec3 emitterRayDirection;
				switch (emitter->getDirection()) {
					case Direction::EAST:      { emitterRayDirection.set( 1.0f,  0.0f, 0.0f); } break;
					case Direction::NORTHEAST: { emitterRayDirection.set( 1.0f,  1.0f, 0.0f); } break;
					case Direction::NORTH:     { emitterRayDirection.set( 0.0f,  1.0f, 0.0f); } break;
					case Direction::NORTHWEST: { emitterRayDirection.set(-1.0f,  1.0f, 0.0f); } break;
					case Direction::WEST:      { emitterRayDirection.set(-1.0f,  0.0f, 0.0f); } break;
					case Direction::SOUTHWEST: { emitterRayDirection.set(-1.0f, -1.0f, 0.0f); } break;
					case Direction::SOUTH:     { emitterRayDirection.set( 0.0f, -1.0f, 0.0f); } break;
					case Direction::SOUTHEAST: { emitterRayDirection.set( 1.0f, -1.0f, 0.0f); } break;
				}
				Ray laserRay(Vec3(emitterRayOrigin.x, emitterRayOrigin.y, 0.0f), emitterRayDirection);
				//log("com.zenprogramming.reflection: emitterRayOrigin = %f, %f", emitterRayOrigin.x, emitterRayOrigin.y);
				//log("com.zenprogramming.reflection: emitterRayDirection = %f, %f", emitterRayDirection.x, emitterRayDirection.y);

				Vec3 intersectionPoint = laserRay.intersects(mirrorPlane);
				float intersectionDist = laserRay.dist(mirrorPlane);

				//log("com.zenprogramming.reflection: mirror[%d] intersectionPoint: %f, %f, %f, dist = %f", mirror->getId(), intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, intersectionDist);

				Rect mirrorBoundingBox = mirror->getBoundingBox();
				AABB mirrorAABB(Vec3(mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), 0.0f), Vec3(mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY(), 0.0f));
				bool rayIntersectsMirror = laserRay.intersects(mirrorAABB);
				//log("com.zenprogramming.reflection: laser intersects mirror[%d]? %d", mirror->getId(), rayIntersectsMirror);

				if (rayIntersectsMirror && intersectionDist < minDistance) {
					closestMirror = mirror;
					minDistance = intersectionDist;
				}
			}
			log("com.zenprogramming.reflection: closest mirror %d is located at (%f, %f)", closestMirror->getId(), closestMirror->getParent()->convertToWorldSpace(closestMirror->getPosition()).x, closestMirror->getParent()->convertToWorldSpace(closestMirror->getPosition()).y);
		}
	}
}

