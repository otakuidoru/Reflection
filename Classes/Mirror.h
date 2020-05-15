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

#ifdef COCOS2D_DEBUG
#include <ostream>
#include <sstream>
#endif
#include <map>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "Direction.h"

class Mirror : public cocos2d::Sprite {
protected:
	const int id;
	bool rotatable;
	bool rotating;
	Direction direction;
	std::map<int, bool> planeReflective;

	Mirror(int id);

public:
	constexpr static float ROTATION_TIME = 2.0f;

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

	cocos2d::Plane getReflectivePlane();
	std::vector<cocos2d::Plane> getPlanes();
	cocos2d::Vec3 getReflectionVector(const cocos2d::Ray& ray);

	bool isPlaneReflective(int index) { return this->planeReflective[index]; }
	cocos2d::Plane getPlane(int index);

	void rotate();

#ifdef COCOS2D_DEBUG
	std::string const str() const {
		std::stringstream ss;
		ss << "{";
		ss << "\t\"id\": " << id << ",";
		ss << "\t\"rotatable\": " << rotatable << ",";
		ss << "\t\"rotating\": " << rotating << ",";
		ss << "\t\"direction\":,";
		ss << "}";
		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream &os, const Mirror& m) { return os << m.str(); }
#endif
};

#endif // __MIRROR_H__

