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
#include <sstream>
#include "tinyxml2.h"
#include "HelloWorldScene.h"
#include "ColorType.h"

#define BACKGROUND_LAYER -1
#define LASER_LAYER 1
#define BLOCK_LAYER 2
#define EMITTER_LAYER 2
#define MIRROR_LAYER 2
#define RECEPTOR_LAYER 2

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Scene* HelloWorld::createScene(const std::string& levelFilename) {
	return HelloWorld::create(levelFilename);
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	log("Error while loading: %s\n", filename);
	log("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

/**
 *
 */
HelloWorld* HelloWorld::create(const std::string& levelFilename) {
	HelloWorld *ret = new (std::nothrow) HelloWorld();
	if (ret && ret->init(levelFilename)) {
		ret->autorelease();
		return ret;
	} else {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

// on "init" you need to initialize your instance
bool HelloWorld::init(const std::string& levelFilename) {
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

	std::stringstream ss;
	ss << FileUtils::getInstance()->getWritablePath() << levelFilename;
	FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("1.xml"), ss.str());
	this->createLevel(ss.str());
	//log("com.zenprogramming.reflection: Filename = %s", ss.str().c_str());

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void HelloWorld::createLevel(const std::string& filename) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	const float scale = std::min(visibleSize.width / 1536.0f, visibleSize.height / 2048.0f);

	tinyxml2::XMLDocument document;
	tinyxml2::XMLError error = document.LoadFile(filename.c_str());
	tinyxml2::XMLElement* level = document.RootElement();
	tinyxml2::XMLElement* levelObjects = level->FirstChildElement("objects");
	tinyxml2::XMLElement* objectElement = levelObjects->FirstChildElement("object");

	while (objectElement) {
		// id
		int objectId;
		objectElement->QueryIntAttribute("id", &objectId);

		// object type
		std::string objectType(objectElement->Attribute("type"));

		// color type
		ColorType colorType = ColorType::NONE;
		Color3B color(255, 255, 255);
		std::string colorTypeStr(objectElement->FirstChildElement("colortype")->GetText());

		if (!colorTypeStr.compare("NONE")) {
			colorType = ColorType::NONE;
		} else if (!colorTypeStr.compare("RED")) {
			colorType = ColorType::RED;
			color = Color3B(255, 153, 153);
		} else if (!colorTypeStr.compare("GREEN")) {
			colorType = ColorType::GREEN;
		} else if (!colorTypeStr.compare("BLUE")) {
			colorType = ColorType::BLUE;
		} else if (!colorTypeStr.compare("YELLOW")) {
			colorType = ColorType::YELLOW;
		} else if (!colorTypeStr.compare("ORANGE")) {
			colorType = ColorType::ORANGE;
		} else if (!colorTypeStr.compare("PURPLE")) {
			colorType = ColorType::PURPLE;
		} else if (!colorTypeStr.compare("WHITE")) {
			colorType = ColorType::WHITE;
		} else if (!colorTypeStr.compare("BLACK")) {
			colorType = ColorType::BLACK;
		}

		// create the object
		GameObject* object = nullptr;
		if (!objectType.compare("block")) {
			object = Block::create(objectId);
			Block* block = dynamic_cast<Block*>(object);
		this->blocks.insert(block);
		} else if (!objectType.compare("emitter")) {
			object = Emitter::create(objectId, colorType);
			Emitter* emitter = dynamic_cast<Emitter*>(object);
			emitter->onAfterActivate = [&]() {
				this->checkWinCondition();
			};
			this->emitters.insert(emitter);
		} else if (!objectType.compare("receptor")) {
			object = Receptor::create(objectId, colorType);
			Receptor* receptor = dynamic_cast<Receptor*>(object);
			this->receptors.insert(receptor);
		} else if (!objectType.compare("mirror")) {
			object = Mirror::create(objectId);
			Mirror* mirror = dynamic_cast<Mirror*>(object);
			mirror->onAfterRotate = [&]() {
				this->checkWinCondition();
			};
			this->mirrors.insert(mirror);
		}

		// set object scale
		object->setScale(scale);

		// set object position
		float x, y;
		tinyxml2::XMLElement* positionElement = objectElement->FirstChildElement("position");
		positionElement->QueryFloatAttribute("x", &x);
		positionElement->QueryFloatAttribute("y", &y);
		object->setPosition(Vec2(x, y));

		// set object rotation
		object->setRotation(std::atof(objectElement->FirstChildElement("rotation")->GetText()));

		// set object direction
		Direction direction = Direction::EAST;
		std::string directionStr(objectElement->FirstChildElement("direction")->GetText());
		object->setDirection(this->stringToDirection(directionStr));

		// set object color
		object->setColor(color);

		this->addChild(object, EMITTER_LAYER);
		this->objects.insert(object);

		objectElement = objectElement->NextSiblingElement();
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	//  Parse the win condition
	//
	//////////////////////////////////////////////////////////////////////////////

	// get the <wincondition> element
	tinyxml2::XMLElement* winConditionElement = level->FirstChildElement("wincondition");

	{ // parse the win condition - emitters
		tinyxml2::XMLElement* winConditionEmittersElement = winConditionElement->FirstChildElement("emitters");
		tinyxml2::XMLElement* winConditionEmitterElement = winConditionEmittersElement->FirstChildElement("emitter");
		while (winConditionEmitterElement) {
			// get emitter id
			int emitterId;
			winConditionEmitterElement->QueryIntAttribute("id", &emitterId);

			// get emitter activity
			const std::string emitterActiveStr(winConditionEmitterElement->Attribute("active"));
			const bool emitterActive = !emitterActiveStr.compare("true");

			for (auto emitter : this->emitters) {
				if (emitter->getId() == emitterId) {
					this->emitterActiveWinConditions[emitter] = emitterActive;
				}
			}

			winConditionEmitterElement = winConditionEmitterElement->NextSiblingElement();
		}
	}

	{ // parse the win condition - mirrors
		tinyxml2::XMLElement* winConditionMirrorsElement = winConditionElement->FirstChildElement("mirrors");
		tinyxml2::XMLElement* winConditionMirrorElement = winConditionMirrorsElement->FirstChildElement("mirror");
		while (winConditionMirrorElement) {
			// get mirror id
			int mirrorId;
			winConditionMirrorElement->QueryIntAttribute("id", &mirrorId);

			// get mirror direction
			const std::string mirrorDirectionStr(winConditionMirrorElement->Attribute("direction"));
			const Direction mirrorDirection = this->stringToDirection(mirrorDirectionStr);

			for (auto mirror : this->mirrors) {
				if (mirror->getId() == mirrorId) {
					this->mirrorDirectionWinConditions[mirror] = mirrorDirection;
				}
			}

			winConditionMirrorElement = winConditionMirrorElement->NextSiblingElement();
		}
	}
}

/**
 *
 */
Direction HelloWorld::stringToDirection(const std::string& str) {
	Direction direction = Direction::EAST;

	if (!str.compare("NORTH")) {
		direction = Direction::NORTH;
	} else if (!str.compare("NORTHEAST")) {
		direction = Direction::NORTHEAST;
	} else if (!str.compare("EAST")) {
		direction = Direction::EAST;
	} else if (!str.compare("SOUTHEAST")) {
		direction = Direction::SOUTHEAST;
	} else if (!str.compare("SOUTH")) {
		direction = Direction::SOUTH;
	} else if (!str.compare("SOUTHWEST")) {
		direction = Direction::SOUTHWEST;
	} else if (!str.compare("WEST")) {
		direction = Direction::WEST;
	} else if (!str.compare("NORTHWEST")) {
		direction = Direction::NORTHWEST;
	}

	return direction;
}

/**
 *
 */
Vec3 HelloWorld::getReflectionVector(const Plane& plane, const Ray& ray) {
	//log("com.zenprogramming.reflection: BEGIN getReflectionVector");
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
	Laser* const laser = Laser::create();
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

		// get the AABB for the object
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
	// check the emitters for win condition
	for (std::map<Emitter*, bool>::iterator itr=emitterActiveWinConditions.begin(); itr!=emitterActiveWinConditions.end(); ++itr) {
		Emitter* const emitter = itr->first;
		if (emitter->isActive() != itr->second) {
			return false;
		}
	}

	// check the mirrors for win condition
	for (std::map<Mirror*, Direction>::iterator itr=mirrorDirectionWinConditions.begin(); itr!=mirrorDirectionWinConditions.end(); ++itr) {
		Mirror* const mirror = itr->first;
		if (mirror->getDirection() != itr->second) {
			return false;
		}
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
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

			// find the closest intersection for the emitter laser
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

