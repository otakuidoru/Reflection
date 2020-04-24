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

#ifndef __EMITTER_H__
#define __EMITTER_H__

#include <functional>
#include "cocos2d.h"
#include "external/Box2D/include/Box2D/Box2D.h"
#include "Laser.h"

class Emitter : public cocos2d::Sprite {
protected:
	int id;
	b2World* world;
	cocos2d::Scene* scene;
	short direction;
	bool active;
	Laser* laser;

	Emitter(int id, b2World* world, cocos2d::Scene* scene);

public:
	static Emitter* create(int id, b2World* world, cocos2d::Scene* scene);
	virtual ~Emitter();

	bool initWithFile(const std::string& filename, b2World* world);

	int getId() const { return id; }

	inline short getDirection() const { return direction; }
	inline void setDirection(short direction) { this->direction = direction % 4; }

	inline bool isActive() const { return active; }
	void setActive(bool active);

	inline Laser* getLaser() const { return laser; }

	std::function<void(Emitter*)> onActivate;
	std::function<void(Emitter*)> onDeactivate;
};

#endif // __EMITTER_H__

