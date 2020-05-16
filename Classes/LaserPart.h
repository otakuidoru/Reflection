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

#ifndef __LASER_PART_H__
#define __LASER_PART_H__

#include "cocos2d.h"

class LaserPart : public cocos2d::Sprite {
protected:
	//cocos2d::Ray ray;
	cocos2d::Vec2 origin;
	cocos2d::Vec3 direction;

	LaserPart();

public:
	static LaserPart* create();
	virtual ~LaserPart();

	virtual bool initWithFile(const std::string& filename) override;

	cocos2d::Ray getRay() const;

	float getLength() const { return this->getScaleX(); }
	void setLength(float length) { this->setScaleX(length); }

//	inline cocos2d::Vec3 getDirection() const { return direction; }
//	inline void setDirection(cocos2d::Vec3 direction) { this->direction = direction; }
};

#endif // __LASER_PART_H__
