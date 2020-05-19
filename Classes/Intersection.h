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

#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include "cocos2d.h"

class Intersection {
protected:
	const cocos2d::Plane plane;
	const cocos2d::Vec3 point;
	const cocos2d::PointSide pointSide;
	const bool planeReflective;
	const float distance;

public:
	Intersection(const cocos2d::Plane& plane, const cocos2d::Vec3& point, cocos2d::PointSide pointSide, bool planeReflective, float distance);
	Intersection(const Intersection& intersection);	// copy constructor
	virtual ~Intersection();

	cocos2d::Plane getPlane() const { return this->plane; }
	cocos2d::Vec3 getPoint() const { return this->point; }
	cocos2d::PointSide getPointSide() const { return this->pointSide; }
	int isPlaneReflective() const { return this->planeReflective; }
	float getDistance() const { return this->distance; }
};

#endif // __INTERSECTION_H__

