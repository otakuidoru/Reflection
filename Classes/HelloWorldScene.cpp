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
	this->addChild(mirror1, 1);
	this->mirrors.insert(mirror1);

	Mirror* mirror2 = Mirror::create(2);
	mirror2->setPosition(Vec2(512, 1024));
	this->addChild(mirror2, 1);
	this->mirrors.insert(mirror2);

	Mirror* mirror3 = Mirror::create(3);
	mirror3->setPosition(Vec2(768, 1280));
	this->addChild(mirror3, 1);
	this->mirrors.insert(mirror3);

	// Add the emitters

	Emitter* emitter = Emitter::create(1);
	emitter->setPosition(Vec2(visibleSize.width/2, emitter->getContentSize().height));
	emitter->setRotation(-90.0f);
	emitter->onActivate = [&](Emitter* e) {};
	emitter->onDeactivate = [&](Emitter* e) {};
	this->addChild(emitter, 1);
	this->emitters.insert(emitter);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void HelloWorld::update(float dt) {
	//log("com.zenprogramming.reflection: HelloWorld::update begin");

	for (auto emitter : this->emitters) {
		if (emitter->getLaser()) {
			std::map<Mirror*, float> mirrorDistanceMap;
			for (auto mirror : this->mirrors) {
				// get the plane
				const Vec2 mirrorPos = mirror->getParent()->convertToWorldSpace(mirror->getPosition());
				log("com.zenprogramming.reflection: mirror[%d] is located at (%f, %f)", mirror->getId(), mirrorPos.x, mirrorPos.y);
				const Size mirrorSize = mirror->getContentSize();
				const float halfMirrorWidth = mirrorSize.width / 2.0f;
				const float halfMirrorHeight = mirrorSize.height / 2.0f;
				const float rotation = mirror->getRotation() * DEGTORAD;
				const float fortyFiveDegrees = 45.0f * DEGTORAD;
				const Vec3 points[3] = {
					Vec3(mirrorPos.x + std::cosf(rotation - fortyFiveDegrees) * halfMirrorWidth, mirrorPos.y + std::sinf(rotation - fortyFiveDegrees) * halfMirrorHeight, 0.0f),
					Vec3(mirrorPos.x + std::cosf(rotation + fortyFiveDegrees) * halfMirrorWidth, mirrorPos.y + std::sinf(rotation + fortyFiveDegrees) * halfMirrorHeight, 0.0f),
					Vec3(mirrorPos.x + std::cosf(rotation + fortyFiveDegrees) * halfMirrorWidth, mirrorPos.y + std::sinf(rotation + fortyFiveDegrees) * halfMirrorHeight, 1.0f),
				};
				Plane mirrorPlane(points[0], points[1], points[2]);
				log("com.zenprogramming.reflection: plane[0] = (%f, %f, %f)", points[0].x, points[0].y, points[0].z);
				log("com.zenprogramming.reflection: plane[1] = (%f, %f, %f)", points[1].x, points[1].y, points[1].z);
				log("com.zenprogramming.reflection: plane[2] = (%f, %f, %f)", points[2].x, points[2].y, points[2].z);

				// get the laser ray
				Vec2 emitterRayOrigin = emitter->getParent()->convertToWorldSpace(this->getPosition());
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

				Vec3 intersectionPoint = laserRay.intersects(mirrorPlane);
				float intersectionDist = laserRay.dist(mirrorPlane);
				mirrorDistanceMap[mirror] = intersectionDist;

				log("com.zenprogramming.reflection: mirror[%d] intersectionPoint: %f, %f, %f, dist = %f", mirror->getId(), intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, intersectionDist);
			}

			// find the closest mirror for the emitter ray
			float minDist = 2560.0f;
			Mirror* minDistMirror = nullptr;

			if (!mirrorDistanceMap.empty()) {
				for (std::map<Mirror*, float>::iterator itr=mirrorDistanceMap.begin(); itr!=mirrorDistanceMap.end(); ++itr) {
					Mirror* mirror = itr->first;
					const float dist = itr->second;
					log("com.zenprogramming.reflection: mirror @ %f, %f = %f", mirror->getPositionX(), mirror->getPositionY(), dist);

					if (dist > 0.0f && dist < minDist) {
						minDist = dist;
						minDistMirror = mirror;
						log("com.zenprogramming.reflection: found a closer mirror @ %f, %f = %f", minDistMirror->getPositionX(), minDistMirror->getPositionY(), minDist);
					}
				}
			}

			if (minDistMirror) {
				log("com.zenprogramming.reflection: closest mirror for id %d @ %f, %f = %f", emitter->getId(), minDistMirror->getPositionX(), minDistMirror->getPositionY(), minDist);
			}
		}
		log("com.zenprogramming.reflection:");
	}
	//log("com.zenprogramming.reflection: HelloWorld::update end");


/*
	const Vec3 intersectionPoint = emitter->getLaser()->getRay()->intersects(*plane);
	const PointSide pointSide = plane->getSide(intersectionPoint);
	if (pointSide == PointSide::FRONT_PLANE) {
		log("com.zenprogramming.reflection: intersection point: %f %f", intersectionPoint.x, intersectionPoint.y);
		distanceToMirrorsMap[distToMirror] = mirror;
	}


	// if we have an intersection, get the mirror with the shortest intersection distance
	if (!distanceToMirrorsMap.empty()) {
		//const Mirror* const closestMirror = distanceToMirrorsMap.begin()->second;
		//log("com.zenprogramming.reflection: closest mirror for id %d = %f %f", emitter->getId(), closestMirror->getPositionX(), closestMirror->getPositionY());
		for (std::map<float, Mirror*>::iterator iter = distanceToMirrorsMap.begin(); iter != distanceToMirrorsMap.end(); ++iter) {
			log("com.zenprogramming.reflection: mirror id %d @ %f %f = %f", emitter->getId(), iter->second->getPositionX(), iter->second->getPositionY(), iter->first);
		}
	}
*/
}

