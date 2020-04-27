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

#ifndef __LASER_H__
#define __LASER_H__

#include "cocos2d.h"
#include "external/Box2D/include/Box2D/Box2D.h"

class Laser : public cocos2d::Sprite {
protected:
	const int id = -1;
	b2World* world = nullptr;
	cocos2d::Ray* ray = nullptr;
	bool updateNeeded = false;
	b2Body* body = nullptr;

	Laser(int id, b2World* world);

	void initBox2D(b2World* const world, const Node* const parent = nullptr);
	void redoBox2D();

public:
	static Laser* create(int id, b2World* world, const cocos2d::Node* const parent = nullptr);
	virtual ~Laser();

	bool initWithFile(const std::string& filename, b2World* world, const Node* const parent = nullptr);

	int getId() const { return id; }

	bool needsUpdate() const { return updateNeeded; }

	b2Body* getBox2DBody() const { return body; }

	virtual void setPosition(const cocos2d::Vec2& position) override;
	virtual void setPositionNormalized(const cocos2d::Vec2& position) override;
	virtual void setPosition(float x, float y) override;
	virtual void setPositionX(float x) override;
	virtual void setPositionY(float y) override;
	virtual void setRotation(float rotation) override;

	virtual void setScaleX(float scaleX) override;
	virtual void setScaleY(float scaleY) override;
	virtual void setScale(float scale) override;
	virtual void setScale(float scaleX, float scaleY) override;

	float dist(const cocos2d::Plane& plane) const;
	cocos2d::Vec3 intersects(const cocos2d::Plane& plane) const;

	virtual void update(float dt) override;
};

#endif // __LASER_H__

