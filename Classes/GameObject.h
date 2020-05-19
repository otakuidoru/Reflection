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

#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <map>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "ColorType.h"
#include "Direction.h"

class GameObject : public cocos2d::Sprite {
protected:
	const int id;
	const unsigned int numPlanes;
	const ColorType colorType;

	Direction direction;
	std::map<int, bool> planeReflective;

	GameObject(int id, ColorType colorType, unsigned int numPlanes);

public:
	virtual ~GameObject();

	virtual bool initWithFile(const std::string& filename) override;

	inline int getId() const { return this->id; }

	inline ColorType getColorType() const { return this->colorType; }

	inline Direction getDirection() const { return this->direction; }
	inline void setDirection(Direction direction) { this->direction = direction; }

	inline unsigned int getNumPlanes() const { return this->numPlanes; }
	virtual cocos2d::Plane getPlane(unsigned int index) = 0;
	std::vector<cocos2d::Plane> getPlanes();
	bool isPlaneReflective(unsigned int index);
	void setPlaneReflective(unsigned int index, bool reflective);
};

#endif // __GAME_OBJECT_H__

