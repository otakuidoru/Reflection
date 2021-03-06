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

#include <string>
#include "cocos2d.h"

class Laser : public cocos2d::Sprite {
protected:
	cocos2d::Vec2 origin;
	cocos2d::Vec3 direction;

	Laser();

public:
	static Laser* create(const cocos2d::Color3B& color);
	virtual ~Laser();

	virtual bool initWithFileAndColor(const std::string& filename, const cocos2d::Color3B& color);

	cocos2d::Ray getRay() const;

//	inline cocos2d::Vec3 getDirection() const { return direction; }
//	inline void setDirection(cocos2d::Vec3 direction) { this->direction = direction; }

#ifdef COCOS2D_DEBUG
	std::string str();
#endif
};

#endif // __LASER_H__

