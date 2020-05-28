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
#include "LevelSelectScene.h"
#include "HelloWorldScene.h"
#include "BackArrow.h"

#define BACKGROUND_LAYER -1
#define LASER_LAYER 1
#define OBJECT_LAYER 2
#define BACK_ARROW_LAYER 255

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

/**
 * on "init" you need to initialize your instance
 */
bool HelloWorld::init(const std::string& levelFilename) {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const float scale = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

	/////////////////////////////
	// 2. add your codes below...

	strToColorTypeMap = {
		{"NONE", ColorType::NONE},
		{"RED", ColorType::RED},
		{"GREEN", ColorType::GREEN},
		{"BLUE", ColorType::BLUE},
		{"YELLOW", ColorType::YELLOW},
		{"ORANGE", ColorType::ORANGE},
		{"PURPLE", ColorType::PURPLE},
		{"WHITE", ColorType::WHITE},
		{"BLACK", ColorType::BLACK}
	};

	colorTypeToObjectColor3BMap = {
		{ColorType::NONE, Color3B(255, 255, 255)},
		{ColorType::RED, Color3B(255, 153, 153)},
		{ColorType::GREEN, Color3B(153, 255, 153)},
		{ColorType::BLUE, Color3B(153, 153, 255)},
		{ColorType::YELLOW, Color3B(255, 255, 255)},	// TODO
		{ColorType::ORANGE, Color3B(255, 255, 255)},	// TODO
		{ColorType::PURPLE, Color3B(255, 255, 255)},	// TODO
		{ColorType::WHITE, Color3B(255, 255, 255)},
		{ColorType::BLACK, Color3B(0, 0, 0)}
	};

	colorTypeToLaserColor3BMap = {
		{ColorType::NONE, Color3B(255, 255, 255)},
		{ColorType::RED, Color3B(255, 0, 0)},
		{ColorType::GREEN, Color3B(0, 255, 0)},
		{ColorType::BLUE, Color3B(0, 0, 255)},
		{ColorType::YELLOW, Color3B(255, 255, 255)},	// TODO
		{ColorType::ORANGE, Color3B(255, 255, 255)},	// TODO
		{ColorType::PURPLE, Color3B(255, 255, 255)},	// TODO
		{ColorType::WHITE, Color3B(255, 255, 255)},
		{ColorType::BLACK, Color3B(0, 0, 0)}
	};

	strToDirectionMap = {
		{"NORTH",     Direction::NORTH    },
		{"NORTHEAST", Direction::NORTHEAST},
		{"EAST",      Direction::EAST     },
		{"SOUTHEAST", Direction::SOUTHEAST},
		{"SOUTH",     Direction::SOUTH    },
		{"SOUTHWEST", Direction::SOUTHWEST},
		{"WEST",      Direction::WEST     },
		{"NORTHWEST", Direction::NORTHWEST},
	};

	auto background = LayerGradient::create(Color4B(253, 158, 246, 255), Color4B(255, 255, 255, 255), Vec2(1.0f, 1.0f));
	this->addChild(background, BACKGROUND_LAYER);

  Director::getInstance()->setClearColor(Color4F(1.0f, 1.0f, 1.0f, 1.0f));

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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define PATH_SEPARATOR "\\"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define PATH_SEPARATOR "/"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define PATH_SEPARATOR "\/"
#endif

	std::stringstream source;
	source << "levels" << PATH_SEPARATOR << levelFilename;
	//log("com.zenprogramming.reflection: Source Filename = %s", source.str().c_str());

	std::stringstream target;
	target << FileUtils::getInstance()->getWritablePath() << levelFilename;
	//log("com.zenprogramming.reflection: Target Filename = %s", target.str().c_str());

	// copy the file to the writable area on the device
	// (have to do this for TinyXML to read the file)
	FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile(source.str()), target.str());
	this->createLevel(target.str());

	// create the back arrow
	auto backArrow = BackArrow::create();
	backArrow->onClick = []() {
		auto levelSelectScene = LevelSelect::create();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, levelSelectScene, Color3B(0, 0, 0)));
	};
	backArrow->setPosition(Vec2(96.0f, 1968.0f));
	this->addChild(backArrow, BACK_ARROW_LAYER);

	this->scheduleUpdate();

	return true;
}

/**
 *
 */
void HelloWorld::addEmitters(tinyxml2::XMLElement* const emittersElement, float scale) {
	if (emittersElement) {
		tinyxml2::XMLElement* emitterElement = emittersElement->FirstChildElement("emitter");

		while (emitterElement) {
			// id
			int id;
			emitterElement->QueryIntAttribute("id", &id);

			// color type
			std::string colorTypeStr(emitterElement->FirstChildElement("colortype")->GetText());
			const ColorType colorType = this->strToColorTypeMap[colorTypeStr];

			// create emitter
			auto emitter = Emitter::create(id, colorType);
			emitter->onAfterActivate = [&]() {
				this->checkWinCondition();
			};
			this->emitters.insert(emitter);
			this->objects.insert(emitter);

			// set emitter color
			const Color3B color = colorTypeToObjectColor3BMap[colorType];
			emitter->setColor(color);

			// set emitter scale
			emitter->setScale(scale);

			// set emitter position
			float x, y;
			tinyxml2::XMLElement* positionElement = emitterElement->FirstChildElement("position");
			positionElement->QueryFloatAttribute("x", &x);
			positionElement->QueryFloatAttribute("y", &y);
			emitter->setPosition(Vec2(x, y));

			// set emitter direction
			std::string directionStr(emitterElement->FirstChildElement("direction")->GetText());
			emitter->setDirection(strToDirectionMap[directionStr]);

			// set emitter active
			std::string activeStr = emitterElement->FirstChildElement("active")->GetText();
			emitter->setActive(!activeStr.compare("true"));

			// add the emitter to the scene
			this->addChild(emitter, OBJECT_LAYER);

			emitterElement = emitterElement->NextSiblingElement("emitter");
		}
	}
}

/**
 *
 */
void HelloWorld::addMirrors(tinyxml2::XMLElement* const mirrorsElement, float scale) {
	if (mirrorsElement) {
		tinyxml2::XMLElement* mirrorElement = mirrorsElement->FirstChildElement("mirror");

		while (mirrorElement) {
			// id
			int id;
			mirrorElement->QueryIntAttribute("id", &id);

			// create mirror
			auto mirror = Mirror::create(id);
			mirror->onAfterRotate = [&]() {
				this->checkWinCondition();
			};
			this->mirrors.insert(mirror);
			this->objects.insert(mirror);

			// set mirror scale
			mirror->setScale(scale);

			// set mirror position
			float x, y;
			tinyxml2::XMLElement* positionElement = mirrorElement->FirstChildElement("position");
			positionElement->QueryFloatAttribute("x", &x);
			positionElement->QueryFloatAttribute("y", &y);
			mirror->setPosition(Vec2(x, y));

			// set mirror direction
			std::string directionStr(mirrorElement->FirstChildElement("direction")->GetText());
			mirror->setDirection(strToDirectionMap[directionStr]);

			// add the mirror to the scene
			this->addChild(mirror, OBJECT_LAYER);

			mirrorElement = mirrorElement->NextSiblingElement("mirror");
		}
	}
}

/**
 *
 */
void HelloWorld::addReceptors(tinyxml2::XMLElement* const receptorsElement, float scale) {
	if (receptorsElement) {
		tinyxml2::XMLElement* receptorElement = receptorsElement->FirstChildElement("receptor");

		while (receptorElement) {
			// id
			int id;
			receptorElement->QueryIntAttribute("id", &id);

			// color type
			std::string colorTypeStr(receptorElement->FirstChildElement("colortype")->GetText());
			const ColorType colorType = this->strToColorTypeMap[colorTypeStr];

			// create receptor
			auto receptor = Receptor::create(id, colorType);
			this->receptors.insert(receptor);
			this->objects.insert(receptor);

			// set receptor color
			const Color3B color = colorTypeToObjectColor3BMap[colorType];
			receptor->setColor(color);

			// set receptor scale
			receptor->setScale(scale);

			// set receptor position
			float x, y;
			tinyxml2::XMLElement* positionElement = receptorElement->FirstChildElement("position");
			positionElement->QueryFloatAttribute("x", &x);
			positionElement->QueryFloatAttribute("y", &y);
			receptor->setPosition(Vec2(x, y));

			// set receptor direction
			std::string directionStr(receptorElement->FirstChildElement("direction")->GetText());
			receptor->setDirection(strToDirectionMap[directionStr]);

			// add the receptor to the scene
			this->addChild(receptor, OBJECT_LAYER);

			receptorElement = receptorElement->NextSiblingElement("receptor");
		}
	}
}

/**
 *
 */
void HelloWorld::addBlocks(tinyxml2::XMLElement* const blocksElement, float scale) {
	if (blocksElement) {
		tinyxml2::XMLElement* blockElement = blocksElement->FirstChildElement("block");

		while (blockElement) {
			// id
			int id;
			blockElement->QueryIntAttribute("id", &id);

			// create block
			auto block = Block::create(id);
			this->blocks.insert(block);
			this->objects.insert(block);

			// set block scale
			block->setScale(scale);

			// set block position
			float x, y;
			tinyxml2::XMLElement* positionElement = blockElement->FirstChildElement("position");
			positionElement->QueryFloatAttribute("x", &x);
			positionElement->QueryFloatAttribute("y", &y);
			block->setPosition(Vec2(x, y));

			// set block direction
			std::string directionStr(blockElement->FirstChildElement("direction")->GetText());
			block->setDirection(strToDirectionMap[directionStr]);

			// add the block to the scene
			this->addChild(block, OBJECT_LAYER);

			blockElement = blockElement->NextSiblingElement("block");
		}
	}
}

/**
 *
 */
void HelloWorld::createLevel(const std::string& filename) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	const float scale = std::min(visibleSize.width/1536.0f, visibleSize.height/2048.0f);

	tinyxml2::XMLDocument document;
	tinyxml2::XMLError error = document.LoadFile(filename.c_str());
	//log("com.zenprogramming.reflection: error = %d", error);

	tinyxml2::XMLElement* levelElement = document.RootElement();

	//////////////////////////////////////////////////////////////////////////////
	//
	//  Add the initial setup objects
	//
	//////////////////////////////////////////////////////////////////////////////

	// add initial objects
	this->addEmitters(levelElement->FirstChildElement("setup")->FirstChildElement("emitters"), scale);
	this->addMirrors(levelElement->FirstChildElement("setup")->FirstChildElement("mirrors"), scale);
	this->addReceptors(levelElement->FirstChildElement("setup")->FirstChildElement("receptors"), scale);
	this->addBlocks(levelElement->FirstChildElement("setup")->FirstChildElement("blocks"), scale);

	//////////////////////////////////////////////////////////////////////////////
	//
	//  Add the win condition
	//
	//////////////////////////////////////////////////////////////////////////////

	// get the <wincondition> element
	tinyxml2::XMLElement* winConditionElement = levelElement->FirstChildElement("wincondition");
	if (winConditionElement) {
		{ // parse the win condition - emitters
			tinyxml2::XMLElement* winConditionEmittersElement = winConditionElement->FirstChildElement("emitters");
			if (winConditionEmittersElement) {
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

					winConditionEmitterElement = winConditionEmitterElement->NextSiblingElement("emitter");
				}
			}
		}
		{ // parse the win condition - mirrors
			tinyxml2::XMLElement* winConditionMirrorsElement = winConditionElement->FirstChildElement("mirrors");
			if (winConditionMirrorsElement) {
				tinyxml2::XMLElement* winConditionMirrorElement = winConditionMirrorsElement->FirstChildElement("mirror");
				while (winConditionMirrorElement) {
					// get mirror id
					int mirrorId;
					winConditionMirrorElement->QueryIntAttribute("id", &mirrorId);

					// get mirror direction
					const std::string mirrorDirectionStr(winConditionMirrorElement->Attribute("direction"));
					const Direction mirrorDirection = strToDirectionMap[mirrorDirectionStr];

					for (auto mirror : this->mirrors) {
						if (mirror->getId() == mirrorId) {
							this->mirrorDirectionWinConditions[mirror] = mirrorDirection;
						}
					}

					winConditionMirrorElement = winConditionMirrorElement->NextSiblingElement("mirror");
				}
			}
		}
	}
}

/**
 * https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
 */
Vec3 HelloWorld::getReflectionVector(const Plane& plane, const Ray& ray) {
	//log("com.zenprogramming.reflection: BEGIN getReflectionVector");
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
void HelloWorld::activateLaserChain(const Ray& originRay, const Vec3& origLaserStartingPoint, const Plane& originPlane, ColorType colorType) {
	//log("com.zenprogramming.reflection: BEGIN activateLaserChain");

	const Vec3 reflectionVector = this->getReflectionVector(originPlane, originRay);
	float angle = -std::atan2(reflectionVector.y, reflectionVector.x) * RADTODEG;
	Ray reflectionRay(origLaserStartingPoint, reflectionVector);

	// create and add a laser
	Laser* const laser = Laser::create(colorTypeToLaserColor3BMap[colorType]);
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
			this->activateLaserChain(laser->getRay(), intersection->getPoint(), intersection->getPlane(), colorType);
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

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite* winBanner = Sprite::create("well_done.png");
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
			Laser* const laser = Laser::create(colorTypeToLaserColor3BMap[emitter->getColorType()]);
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
					this->activateLaserChain(laser->getRay(), intersection->getPoint(), intersection->getPlane(), emitter->getColorType());
				}
			}
		}
	}

	//log("com.zenprogramming.reflection: END update");
}

