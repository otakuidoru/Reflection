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
	this->addChild(background, BACKGROUND_LAYER);

	// add a label that shows "Hello World" and create and initialize a label
	auto label = Label::createWithTTF("Reflection", "fonts/motioncontrol-bold.ttf", 96);
	if (label == nullptr) {
		problemLoading("'fonts/motioncontrol-bold.ttf'");
	} else {
		// position the label on the top center of the screen
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

	Mirror* mirror4 = Mirror::create(4);
	mirror4->setPosition(Vec2(512, 1280));
	this->addChild(mirror4, MIRROR_LAYER);
	this->mirrors.insert(mirror4);

	// Add the emitters

	Emitter* emitter = Emitter::create(1);
	emitter->setPosition(Vec2(visibleSize.width/2, emitter->getContentSize().height));
	emitter->setRotation(-90.0f);
	emitter->setDirection(Direction::NORTH);
	//emitter->setOpacity(64);
	this->addChild(emitter, EMITTER_LAYER);
	this->emitters.insert(emitter);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
std::shared_ptr<Intersection> HelloWorld::getIntersection(Mirror* const mirror, const Ray& ray) {
	std::shared_ptr<Intersection> intersection;

	const Rect mirrorBoundingBox = mirror->getBoundingBox();

	// determine the closest intersected plane in the bounding box
	float closestIntersectionDistance = 2560.0f;

	std::vector<cocos2d::Plane> mirrorPlanes = mirror->getPlanes();
	for (int planeIndex=0; planeIndex<3; ++planeIndex) {
		// ray intersects the mirror AABB; find the closest intersected plane
		Plane plane = mirror->getPlane(planeIndex);

		const float intersectionDist = ray.dist(plane);
		if (intersectionDist > 0.0f && intersectionDist < closestIntersectionDistance) {
			Vec3 intersectionPoint = ray.intersects(plane);

			if (mirrorBoundingBox.containsPoint(Vec2(intersectionPoint.x, intersectionPoint.y))) {
				const PointSide intersectionPointSide = plane.getSide(ray._origin);
				const bool planeReflective = mirror->isPlaneReflective(planeIndex);

				//log("com.zenprogramming.reflection: intersectionPoint: %f, %f, %f, dist = %f", intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, intersectionDist);

				intersection.reset(new Intersection(plane, planeIndex, intersectionPoint, intersectionPointSide, planeReflective, intersectionDist));

				closestIntersectionDistance = intersectionDist;
			}
		}
	}

	return intersection;
}

/**
 *
 */
void HelloWorld::activateLaserChain(Mirror* const originMirror, const Ray& originRay, const Vec3& origLaserStartingPoint) {
	const Vec3 reflectionVector = originMirror->getReflectionVector(originRay);
	float angle = -std::atan2(reflectionVector.y, reflectionVector.x) * RADTODEG;
	Ray reflectionRay(origLaserStartingPoint, reflectionVector);

	// create and add a laser
	LaserPart* laser = LaserPart::create();
	laser->setPosition(Vec2(origLaserStartingPoint.x, origLaserStartingPoint.y));
	laser->setRotation(angle);
	//log("com.zenprogramming.reflection: laser angle = %f", laser->getRotation());
	laser->setAnchorPoint(Vec2(0.0f, 0.5f));
	laser->setScaleX(2560.0f);
	this->addChild(laser, LASER_LAYER);
	this->lasers.insert(laser);

	Mirror* closestMirror = this->getClosestMirror(originMirror, reflectionRay);

	if (closestMirror) {
		std::shared_ptr<Intersection> intersection = this->getIntersection(closestMirror, laser->getRay());

		if (intersection.get()) {
			//log("com.zenprogramming.reflection: intersection pos: (%f, %f, %f)", intersection->getPoint().x, intersection->getPoint().y, intersection->getPoint().z);
			//log("com.zenprogramming.reflection: intersection dist: %f", intersection->getDistance());

			laser->setScaleX(intersection->getDistance());

			if (intersection->isPlaneReflective()) {
				this->activateLaserChain(closestMirror, laser->getRay(), intersection->getPoint());
			}
		}
	}
}

/**
 *
 */
Mirror* HelloWorld::getClosestMirror(Emitter* const emitter, const Ray& emitterLaserRay) {
	Mirror* closestMirror = nullptr;
	float minDistance = 2560.0f;

	for (auto mirror : this->mirrors) {
		//log("com.zenprogramming.reflection: checking mirror[%d]", mirror->getId());
		const Rect mirrorBoundingBox = mirror->getBoundingBox();
		const AABB mirrorAABB(Vec3(mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), 0.0f), Vec3(mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY(), 0.0f));
		//log("com.zenprogramming.reflection: mirror[%d] AABB = (%f, %f, %f, %f)", mirror->getId(), mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY());

		float intersectionDist;
		const bool rayIntersectsMirror = emitterLaserRay.intersects(mirrorAABB, &intersectionDist);
		//log("com.zenprogramming.reflection: emitter[%d] laser intersects mirror[%d]? %d", emitter->getId(), mirror->getId(), rayIntersectsMirror);

		if (rayIntersectsMirror && intersectionDist < minDistance) {
			closestMirror = mirror;
			minDistance = intersectionDist;
			//log("com.zenprogramming.reflection: emitter[%d] laser hits mirror[%d] at distance = %f", emitter->getId(), closestMirror->getId(), intersectionDist);
		}
	}

	//log("com.zenprogramming.reflection: closest mirror = %d", closestMirror->getId());

	return closestMirror;
}

/**
 *
 */
Mirror* HelloWorld::getClosestMirror(Mirror* const origMirror, const Ray& mirrorLaserRay) {
	Mirror* closestMirror = nullptr;
	float minDistance = 2560.0f;

	for (auto mirror : this->mirrors) {
		if (mirror != origMirror) {
			//log("com.zenprogramming.reflection: checking mirror[%d]", mirror->getId());
			const Rect mirrorBoundingBox = mirror->getBoundingBox();
			const AABB mirrorAABB(Vec3(mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), 0.0f), Vec3(mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY(), 0.0f));
			//log("com.zenprogramming.reflection: mirror[%d] AABB = (%f, %f, %f, %f)", mirror->getId(), mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY());

			float intersectionDist;
			//log("com.zenprogramming.reflection: laser origin (%f, %f), direction (%f, %f, %f)", mirrorLaserRay._origin.x, mirrorLaserRay._origin.y, mirrorLaserRay._direction.x, mirrorLaserRay._direction.y, mirrorLaserRay._direction.z);
			const bool rayIntersectsMirror = mirrorLaserRay.intersects(mirrorAABB, &intersectionDist);
			//log("com.zenprogramming.reflection: origMirror[%d] laser intersects mirror[%d]? %d", origMirror->getId(), mirror->getId(), rayIntersectsMirror);

			if (rayIntersectsMirror && intersectionDist < minDistance) {
				closestMirror = mirror;
				minDistance = intersectionDist;
				//log("com.zenprogramming.reflection: origMirror[%d] laser hits mirror[%d] at distance = %f", origMirror->getId(), closestMirror->getId(), intersectionDist);
				//log("com.zenprogramming.reflection: closest mirror is now %d", closestMirror->getId());
			}
		}
	}

	return closestMirror;
}

/**
 *
 */
void HelloWorld::update(float dt) {
	log("com.zenprogramming.reflection: UPDATE");

	// remove all lasers from the board
	for (auto laser : this->lasers) {
		laser->removeFromParent();
	}
	this->lasers.clear();

	// regenerate lasers on the board
	for (auto emitter : this->emitters) {
		if (emitter->isActive()) {
			const Vec2 emitterWorldPos = emitter->getParent()->convertToWorldSpace(emitter->getPosition());

			// create and add a laser
			LaserPart* laser = LaserPart::create();
			laser->setPosition(Vec2(
				emitterWorldPos.x + (std::cosf(emitter->getRotation() * DEGTORAD) * (-emitter->getContentSize().width / 2.0f)),
				emitterWorldPos.y + (std::sinf(emitter->getRotation() * DEGTORAD) * (-emitter->getContentSize().height / 2.0f))
			));
			laser->setRotation(emitter->getRotation());
			laser->setAnchorPoint(Vec2(0.0f, 0.5f));
			laser->setScaleX(2560.0f);
			this->addChild(laser, LASER_LAYER);
			this->lasers.insert(laser);

			// find the closest mirror for the emitter laser
			Mirror* const closestMirror = this->getClosestMirror(emitter, laser->getRay());
			if (closestMirror) {
				std::shared_ptr<Intersection> intersection = this->getIntersection(closestMirror, laser->getRay());
				//log("com.zenprogramming.reflection: intersection dist: %f", intersection->getDistance());

				if (intersection.get()) {
					const Vec2 mirrorWorldPos = closestMirror->getParent()->convertToWorldSpace(closestMirror->getPosition());
					laser->setScaleX(intersection->getDistance());

					//const Rect mirrorBoundingBox = closestMirror->getBoundingBox();
					//const AABB mirrorAABB(Vec3(mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), 0.0f), Vec3(mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY(), 0.0f));
					//log("com.zenprogramming.reflection: mirror[%d] AABB = (%f, %f, %f, %f)", closestMirror->getId(), mirrorBoundingBox.getMinX(), mirrorBoundingBox.getMinY(), mirrorBoundingBox.getMaxX(), mirrorBoundingBox.getMaxY());
					//log("com.zenprogramming.reflection: laser origin (%f, %f), direction (%f, %f, %f)", laserPart->getRay()._origin.x, laserPart->getRay()._origin.y, laserPart->getRay()._direction.x, laserPart->getRay()._direction.y, laserPart->getRay()._direction.z);
					//log("com.zenprogramming.reflection: laser->getScaleX() = %f", laserPart->getScaleX());

					if (intersection->isPlaneReflective()) {
						this->activateLaserChain(closestMirror, laser->getRay(), intersection->getPoint());
					}
				}
			}
		}
	}
}

