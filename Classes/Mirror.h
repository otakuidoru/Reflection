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

#include "cocos2d.h"

class Mirror : public cocos2d::Node {
protected:
	bool rotatable;
	bool rotating;
	short direction;
	cocos2d::Sprite* sprite;

	Mirror();

	void rotate(bool right);

public:
	constexpr static float ROTATION_TIME = 0.5f;

	// implement the "static create()" method manually
	CREATE_FUNC(Mirror);
	virtual ~Mirror();

	virtual bool init() override;

	bool isRotatable() const { return rotatable; }
	void setRotatable(bool rotatable) { this->rotatable = rotatable; }

	bool isRotating() const { return rotating; }
	void setRotating(bool rotating) { this->rotating = rotating; }

	short getDirection() const { return direction; }

	void rotateCounterclockwise();
	void rotateClockwise();

//	virtual void update(float dt) override;
};

#endif // __MIRROR_H__

