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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "tinyxml2.h"
#include "cocos2d.h"
#include "Block.h"
#include "ColorType.h"
#include "Combiner.h"
#include "Direction.h"
#include "Emitter.h"
#include "GameObject.h"
#include "Intersection.h"
#include "IntroLayerMultiplex.h"
#include "Laser.h"
#include "Mirror.h"
#include "Receptor.h"
#include "Portal.h"
#include "Splitter.h"
#include "BonusStar.h"
#include "WinCondition.h"

class HelloWorld : public cocos2d::Scene {
protected:
	std::map<std::string, ColorType> strToColorTypeMap;
	std::map<ColorType, cocos2d::Color3B> colorTypeToObjectColor3BMap;
	std::map<ColorType, cocos2d::Color3B> colorTypeToLaserColor3BMap;
	std::map<std::string, Direction> strToDirectionMap;

	std::set<GameObject*> objects;
	std::set<Block*> blocks;
	std::set<Emitter*> emitters;
	std::set<Laser*> lasers;
	std::set<Mirror*> mirrors;
	std::set<Receptor*> receptors;
	std::set<Portal*> portals;
	std::set<Combiner*> combiners;
	std::set<Splitter*> splitters;
	std::set<BonusStar*> bonusStars;

	IntroLayerMultiplex* introLayers;

	std::vector<std::shared_ptr<WinCondition>> winConditions;

	int levelId;
	int worldId;
	std::string levelFilename;
	bool ready;

	// level methods
	cocos2d::Vec3 getReflectionVector(const cocos2d::Plane& plane, const cocos2d::Ray& ray);
	std::shared_ptr<Intersection> getClosestIntersection(const cocos2d::Ray& ray);
	void activateLaserChain(const cocos2d::Ray& originRay, const cocos2d::Vec3& origLaserStartingPoint, const cocos2d::Plane& originPlane, ColorType colorType);

	// level creation methods
	void addEmitters(tinyxml2::XMLElement* const emittersElement, float scale);
	void addMirrors(tinyxml2::XMLElement* const mirrorsElement, float scale);
	void addReceptors(tinyxml2::XMLElement* const receptorsElement, float scale);
	void addBlocks(tinyxml2::XMLElement* const blocksElement, float scale);
	void addBonusStars(tinyxml2::XMLElement* const bonusStarsElement, float scale);
	void addPortals(tinyxml2::XMLElement* const portalsElement, float scale);
	void addCombiners(tinyxml2::XMLElement* const combinersElement, float scale);
	void addSplitters(tinyxml2::XMLElement* const splittersElement, float scale);
	void createLevel(const std::string& filename);

	bool checkWinConditions();
	Laser* addLaser(float angle, const cocos2d::Vec2& position, const cocos2d::Color3B& color);

public:
	static cocos2d::Scene* createScene(const std::string& levelFilename, int levelId, int worldId);

	// implement the "static create()" method manually
	static HelloWorld* create(const std::string& levelFilename, int levelId, int worldId);
	virtual bool init(const std::string& levelFilename, int levelId, int worldId);

	virtual void update(float dt) override;
};

#endif // __HELLOWORLD_SCENE_H__

