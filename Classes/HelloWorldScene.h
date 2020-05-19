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

#include <memory>
#include <set>
#include "cocos2d.h"
#include "Emitter.h"
#include "Intersection.h"
#include "GameObject.h"
#include "Laser.h"
#include "Mirror.h"
#include "Receptor.h"

class HelloWorld : public cocos2d::Scene {
protected:
	std::set<GameObject*> objects;
	std::set<Emitter*> emitters;
	std::set<Laser*> lasers;
	std::set<Mirror*> mirrors;
	std::set<Receptor*> receptors;

	cocos2d::Vec3 getReflectionVector(const cocos2d::Plane& plane, const cocos2d::Ray& ray);
	std::shared_ptr<Intersection> getIntersection(GameObject* const object, const cocos2d::Ray& ray);
	std::shared_ptr<Intersection> getClosestIntersection(const cocos2d::Ray& ray);
	void activateLaserChain(const cocos2d::Ray& originRay, const cocos2d::Vec3& origLaserStartingPoint, const cocos2d::Plane& originPlane);

public:
	static cocos2d::Scene* createScene();

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
	virtual bool init() override;

	bool checkWinCondition();

	virtual void update(float dt) override;
};

#endif // __HELLOWORLD_SCENE_H__

