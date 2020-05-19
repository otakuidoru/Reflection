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
#define RECEPTOR_LAYER 2

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
	const float scale = std::min(visibleSize.width / 1536.0f, visibleSize.height / 2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	auto background = LayerGradient::create(Color4B(253, 158, 246, 255), Color4B(255, 255, 255, 255), Vec2(1.0f, 1.0f));
	this->addChild(background, BACKGROUND_LAYER);

//	// add a label that shows "Hello World" and create and initialize a label
//	auto label = Label::createWithTTF("Reflection", "fonts/motioncontrol-bold.ttf", 96);
//	if (label == nullptr) {
//		problemLoading("'fonts/motioncontrol-bold.ttf'");
//	} else {
//		// position the label on the top center of the screen
//		label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
//
//		// add the label as a child to this layer
//		this->addChild(label, 1);
//	}

	// Add the emitters

	Emitter* const emitter = Emitter::create(1, ColorType::RED);
	emitter->setScale(scale);
	emitter->setPosition(Vec2(768, 128));
	emitter->setRotation(-90.0f);
	emitter->setDirection(Direction::NORTH);
	emitter->setColor(Color3B(255, 153, 153));
	emitter->onAfterActivate = [&]() {
		this->checkWinCondition();
	};
	this->addChild(emitter, EMITTER_LAYER);
	this->emitters.insert(emitter);
	this->objects.insert(emitter);

	// Add the receptors

	Receptor* const receptor = Receptor::create(2, ColorType::RED);
	receptor->setScale(scale);
	receptor->setPosition(Vec2(768, 1792));
	receptor->setDirection(Direction::NORTH);
	receptor->setColor(Color3B(255, 153, 153));
	this->addChild(receptor, RECEPTOR_LAYER);
	this->receptors.insert(receptor);
	this->objects.insert(receptor);

	// Add the mirrors

	Mirror* const mirror1 = Mirror::create(3);
	mirror1->setScale(scale);
	mirror1->setPosition(Vec2(512, 1280));
	mirror1->onAfterRotate = [&]() {
		this->checkWinCondition();
	};
	this->addChild(mirror1, MIRROR_LAYER);
	this->mirrors.insert(mirror1);
	this->objects.insert(mirror1);

	Mirror* const mirror2 = Mirror::create(4);
	mirror2->setScale(scale);
	mirror2->setPosition(Vec2(768, 1280));
	mirror2->onAfterRotate = [&]() {
		this->checkWinCondition();
	};
	this->addChild(mirror2, MIRROR_LAYER);
	this->mirrors.insert(mirror2);
	this->objects.insert(mirror2);

	Mirror* const mirror3 = Mirror::create(5);
	mirror3->setScale(scale);
	mirror3->setPosition(Vec2(512, 1024));
	mirror3->onAfterRotate = [&]() {
		this->checkWinCondition();
	};
	this->addChild(mirror3, MIRROR_LAYER);
	this->mirrors.insert(mirror3);
	this->objects.insert(mirror3);

	Mirror* const mirror4 = Mirror::create(6);
	mirror4->setScale(scale);
	mirror4->setPosition(Vec2(768, 1024));
	mirror4->onAfterRotate = [&]() {
		this->checkWinCondition();
	};
	this->addChild(mirror4, MIRROR_LAYER);
	this->mirrors.insert(mirror4);
	this->objects.insert(mirror4);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
Vec3 HelloWorld::getReflectionVector(const Plane& plane, const Ray& ray) {
	log("com.zenprogramming.reflection: BEGIN getReflectionVector");
	// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	const Vec3 d = ray._direction;
	const Vec3 n = plane.getNormal();
	const Vec3 reflectionVector = d - 2 * (d.dot(n)) * n;
	//log("com.zenprogramming.reflection: reflectionVector = (%f, %f, %f)", reflectionVector.x, reflectionVector.y, reflectionVector.z);
	//log("com.zenprogramming.reflection: END getReflectionVector");
	return reflectionVector;
}

/**
 *
 */
std::shared_ptr<Intersection> HelloWorld::getIntersection(GameObject* const object, const Ray& ray) {
	//log("com.zenprogramming.reflection: BEGIN getIntersection");
	//log("com.zenprogramming.reflection: object = %d", object->getId());
	//log("com.zenprogramming.reflection: ray = origin (%f, %f), direction (%f, %f)", ray._origin.x, ray._origin.y, ray._direction.x, ray._direction.y);
	std::shared_ptr<Intersection> intersection;

	const Rect objectBoundingBox = object->getBoundingBox();

	// determine the closest intersected plane in the bounding box
	float closestIntersectionDistance = 2560.0f;

	std::vector<cocos2d::Plane> objectPlanes = object->getPlanes();
	for (int planeIndex=0; planeIndex<3; ++planeIndex) {
		// ray intersects the mirror AABB; find the closest intersected plane
		Plane plane = object->getPlane(planeIndex);

		const float intersectionDist = ray.dist(plane);
		if (intersectionDist > 0.0f && intersectionDist < closestIntersectionDistance) {
			Vec3 intersectionPoint = ray.intersects(plane);

			if (objectBoundingBox.containsPoint(Vec2(intersectionPoint.x, intersectionPoint.y))) {
				const PointSide intersectionPointSide = plane.getSide(ray._origin);
				const bool planeReflective = object->isPlaneReflective(planeIndex);

				//log("com.zenprogramming.reflection: intersectionPoint: %f, %f, %f, dist = %f", intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, intersectionDist);

				intersection.reset(new Intersection(plane, intersectionPoint, intersectionPointSide, planeReflective, intersectionDist));

				closestIntersectionDistance = intersectionDist;
			}
		}
	}

	//log("com.zenprogramming.reflection: END getIntersection");
	return intersection;
}

/**
 *
 */
void HelloWorld::activateLaserChain(const Ray& originRay, const Vec3& origLaserStartingPoint, const Plane& originPlane) {
	//log("com.zenprogramming.reflection: BEGIN activateLaserChain");
	const Vec3 reflectionVector = this->getReflectionVector(originPlane, originRay);
	float angle = -std::atan2(reflectionVector.y, reflectionVector.x) * RADTODEG;
	Ray reflectionRay(origLaserStartingPoint, reflectionVector);

	// create and add a laser
	Laser* laser = Laser::create();
	laser->setPosition(Vec2(origLaserStartingPoint.x, origLaserStartingPoint.y));
	laser->setRotation(angle);
	laser->setAnchorPoint(Vec2(0.0f, 0.5f));
	laser->setScaleX(2560.0f);
	this->addChild(laser, LASER_LAYER);
	this->lasers.insert(laser);

	std::shared_ptr<Intersection> intersection = this->getClosestIntersection(reflectionRay);
	if (intersection.get()) {
		laser->setScaleX(intersection->getDistance());

		if (intersection->isPlaneReflective()) {
			this->activateLaserChain(laser->getRay(), intersection->getPoint(), intersection->getPlane());
		}
	}

	//log("com.zenprogramming.reflection: END activateLaserChain");
}

/**
 *
 */
std::shared_ptr<Intersection> HelloWorld::getClosestIntersection(const Ray& ray) {
	//log("com.zenprogramming.reflection: BEGIN getClosestIntersection");
	std::shared_ptr<Intersection> intersection;
	float minDistance = 2560.0f;

	//log("com.zenprogramming.reflection: ray = origin (%f, %f, %f), direction (%f, %f, %f)", ray._origin.x, ray._origin.y, ray._origin.z, ray._direction.x, ray._direction.y, ray._direction.z);

	// check each object
	for (auto object : this->objects) {
		//log("com.zenprogramming.reflection: checking object[%d]...", object->getId());

		const Rect objectBoundingBox = object->getBoundingBox();
		const AABB objectAABB(Vec3(objectBoundingBox.getMinX(), objectBoundingBox.getMinY(), 0.0f), Vec3(objectBoundingBox.getMaxX(), objectBoundingBox.getMaxY(), 0.0f));
		//log("com.zenprogramming.reflection: object[%d] AABB = (%f, %f, %f, %f), width = %f, height = %f", object->getId(), objectBoundingBox.getMinX(), objectBoundingBox.getMinY(), objectBoundingBox.getMaxX(), objectBoundingBox.getMaxY(), objectBoundingBox.getMaxX() - objectBoundingBox.getMinX(), objectBoundingBox.getMaxY() - objectBoundingBox.getMinY());

		for (int planeIndex=0; planeIndex<object->getNumPlanes(); ++planeIndex) {
			const Plane plane = object->getPlane(planeIndex);
			const Vec3 intersectionPoint = ray.intersects(plane);
			const float intersectionDist = intersectionPoint.distance(ray._origin);
			const bool intersects = ray.intersects(objectAABB);
			//log("com.zenprogramming.reflection: intersects? %d, dist = %f", intersects, intersectionDist);
			if (!intersectionPoint.isZero() && ray.intersects(objectAABB) && objectBoundingBox.containsPoint(Vec2(intersectionPoint.x, intersectionPoint.y))) {
				// ray intersects plane in the object's bounding box
				if (intersectionDist < minDistance) {
					minDistance = intersectionDist;
					intersection.reset(new Intersection(plane, intersectionPoint, plane.getSide(ray._origin), object->isPlaneReflective(planeIndex), intersectionDist));
					//log("com.zenprogramming.reflection: laser hits object[%d] at distance = %f at point (%f, %f, %f)", object->getId(), intersectionDist, intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
					//log("com.zenprogramming.reflection: closest object is now %d", object->getId());
				}
			}
		}
	}

	//log("com.zenprogramming.reflection: END getClosestIntersection");
	return intersection;
}

/**
 *
 */
bool HelloWorld::checkWinCondition() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Emitter* emitter1;

	Mirror* mirror1;
	Mirror* mirror2;
	Mirror* mirror3;
	Mirror* mirror4;

	for (auto emitter : this->emitters) {
		if (emitter->getId() == 1) {
			emitter1 = emitter;
		}
	}

	for (auto mirror : this->mirrors) {
		if (mirror->getId() == 3) {
			mirror1 = mirror;
		} else if (mirror->getId() == 4) {
			mirror2 = mirror;
		} else if (mirror->getId() == 5) {
			mirror3 = mirror;
		} else if (mirror->getId() == 6) {
			mirror4 = mirror;
		}
	}

	if (emitter1->isActive() &&
			mirror1->getDirection() == Direction::SOUTHEAST &&	// 3
			mirror2->getDirection() == Direction::NORTHWEST &&	// 7
			mirror3->getDirection() == Direction::NORTHEAST &&	// 1
			mirror4->getDirection() == Direction::SOUTHWEST			// 5
	) {
		auto winBanner = Sprite::create("well_done.png");
		winBanner->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		winBanner->setOpacity(0);
		winBanner->setScale(5.0f);
		this->addChild(winBanner, 10);
		winBanner->runAction(Spawn::create(
			FadeIn::create(0.5f),
			ScaleTo::create(0.5f, 1.5f),
			nullptr
		));

		return true;
	}

	return false;
}

/**
 *
 */
void HelloWorld::update(float dt) {
	//log("com.zenprogramming.reflection: BEGIN update");

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
			Laser* const laser = Laser::create();
			laser->setPosition(emitterWorldPos);
			laser->setRotation(emitter->getRotation());
			laser->setAnchorPoint(Vec2(0.0f, 0.5f));
			laser->setScaleX(2560.0f);
			this->addChild(laser, LASER_LAYER);
			this->lasers.insert(laser);

			// find the closest object for the emitter laser
			std::shared_ptr<Intersection> intersection = this->getClosestIntersection(laser->getRay());
			if (intersection.get()) {
				laser->setScaleX(intersection->getDistance());

				//log("com.zenprogramming.reflection: laser origin (%f, %f), direction (%f, %f, %f), length = %f", laser->getRay()._origin.x, laser->getRay()._origin.y, laser->getRay()._direction.x, laser->getRay()._direction.y, laser->getRay()._direction.z, laser->getScaleX());
				//log("com.zenprogramming.reflection: intersection at (%f, %f, %f) side = %d, reflective = %d, distance = %f", intersection->getPoint().x, intersection->getPoint().y, intersection->getPoint().z, intersection->getPointSide(), intersection->isPlaneReflective(), intersection->getDistance());

				if (intersection->isPlaneReflective()) {
					this->activateLaserChain(laser->getRay(), intersection->getPoint(), intersection->getPlane());
				}
			}
		}
	}

	//log("com.zenprogramming.reflection: END update");
}

