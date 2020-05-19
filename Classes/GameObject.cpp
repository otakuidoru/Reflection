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

#include "GameObject.h"

USING_NS_CC;

/**
 *
 */
GameObject::GameObject(int id, ColorType colorType, unsigned int numPlanes) : Sprite(), id(id), colorType(colorType), numPlanes(numPlanes) {
}

/**
 *
 */
GameObject::~GameObject() {
}

/**
 * on "init" you need to initialize your instance
 */
bool GameObject::initWithFile(const std::string& filename) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->direction = Direction::EAST;

	return true;
}

/**
 *
 */
std::vector<Plane> GameObject::getPlanes() {
	std::vector<Plane> planes;

	for (unsigned int i=0; i<this->getNumPlanes(); ++i) {
		planes.push_back(this->getPlane(i));
	}

	return planes;
}

/**
 *
 */
bool GameObject::isPlaneReflective(unsigned int index) {
	if (index < this->getNumPlanes()) {
		return this->planeReflective[index];
	}

	return false;
}

/**
 *
 */
void GameObject::setPlaneReflective(unsigned int index, bool reflective) {
	if (index < this->getNumPlanes()) {
		this->planeReflective[index] = reflective;
	}
}

