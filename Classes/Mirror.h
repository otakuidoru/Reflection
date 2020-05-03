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

#ifndef __MIRROR_H__
#define __MIRROR_H__

#include <string>
#include "cocos2d.h"
#include "Direction.h"
#include "Laser.h"

class Mirror : public cocos2d::Sprite {
protected:
	const int id;
	bool rotatable;
	bool rotating;
	Direction direction;
	Laser* laser;
	cocos2d::Plane* plane;

	Mirror(int id);

public:
	constexpr static float ROTATION_TIME = 0.25f;

	static Mirror* create(int id);
	virtual ~Mirror();

	virtual bool initWithFile(const std::string& filename) override;

	inline int getId() const { return id; }

	inline bool isRotatable() const { return rotatable; }
	inline void setRotatable(bool rotatable) { this->rotatable = rotatable; }

	inline bool isRotating() const { return rotating; }
	inline void setRotating(bool rotating) { this->rotating = rotating; }

	inline Direction getDirection() const { return direction; }
	inline void setDirection(Direction direction) { this->direction = direction; }

	inline Laser* getLaser() const { return laser; }

	inline cocos2d::Plane* getReflectivePlane() const { return plane; }

	void startReflect(Laser* originatingLaser);
	void stopReflect(Laser* originatingLaser);

//	virtual float getReflectionNormal() { return reflectionNormal; }

	std::function<void(Mirror*)> onLaserEnter;
	std::function<void(Mirror*)> onLaserExit;

	virtual void update(float dt) override;
};

#endif // __MIRROR_H__

